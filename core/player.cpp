#include "player.h"

#include <cmath>
#include <algorithm>

namespace {
    constexpr float GRAVITY = 40.0f;           
    constexpr float MAX_FALL_SPEED = 24.0f;      

    constexpr float GROUND_MAX_SPEED = 8.0f;     
    constexpr float AIR_MAX_SPEED = 8.0f;       
    constexpr float GROUND_ACCEL = 60.0f;        
    constexpr float AIR_ACCEL = 20.0f;           
    constexpr float GROUND_FRICTION = 0.91f;     
    constexpr float AIR_FRICTION = 0.98f;       

    constexpr float JUMP_SPEED = -14.0f;         
    constexpr int   MAX_JUMPS = 2;            

    constexpr float HOOK_FIRE_SPEED = 26.0f;     
    constexpr float HOOK_MAX_LENGTH = 10.0f;    
    constexpr float HOOK_STEP = 0.1f;           
    constexpr float HOOK_DRAG_ACCEL = 45.0f;    
    constexpr float HOOK_DRAG_MAX_SPEED = 18.0f; 
    constexpr float HOOK_MIN_DIST = 0.5f;      

    constexpr int FREEZE_TICKS = 150;           
}

float Vec2::length() const {
    return std::sqrt(x * x + y * y);
}

Vec2 Vec2::normalized() const {
    float len = length();
    if (len < 0.0001f) return {0.0f, 0.0f};
    return {x / len, y / len};
}

Player::Player() = default;

void Player::spawn(Vec2 pos) {
    m_pos = pos;
    m_vel = {0.0f, 0.0f};
    m_grounded = false;
    m_jumpsLeft = MAX_JUMPS;
    m_hookState = HookState::Idle;
    m_freezeTicks = 0;
}

bool Player::isSolid(const Map& map, int tx, int ty) const {
    if (tx < 0 || ty < 0 || tx >= Map::WIDTH || ty >= Map::HEIGHT) return true; // harita dışı = duvar
    return map.getTile(tx, ty) == Map::WALL;
}

bool Player::boxCollides(const Map& map, Vec2 pos) const {
    int minX = static_cast<int>(std::floor(pos.x - RADIUS));
    int maxX = static_cast<int>(std::floor(pos.x + RADIUS));
    int minY = static_cast<int>(std::floor(pos.y - RADIUS));
    int maxY = static_cast<int>(std::floor(pos.y + RADIUS));

    for (int ty = minY; ty <= maxY; ty++) {
        for (int tx = minX; tx <= maxX; tx++) {
            if (isSolid(map, tx, ty)) return true;
        }
    }
    return false;
}

void Player::applyMovement(const PlayerInput& input, float dt) {
    if (m_freezeTicks > 0) return;

    float accel = m_grounded ? GROUND_ACCEL : AIR_ACCEL;
    float maxSpeed = m_grounded ? GROUND_MAX_SPEED : AIR_MAX_SPEED;
    float friction = m_grounded ? GROUND_FRICTION : AIR_FRICTION;

    m_vel.x *= friction;

    float wish = 0.0f;
    if (input.left) wish -= 1.0f;
    if (input.right) wish += 1.0f;

    if (wish != 0.0f) {
        m_vel.x += wish * accel * dt;
        m_vel.x = std::clamp(m_vel.x, -maxSpeed, maxSpeed);
    }

    if (input.jumpPressed && m_jumpsLeft > 0) {
        m_vel.y = JUMP_SPEED;
        m_jumpsLeft--;
    }
}

void Player::applyGravity(float dt) {
    if (m_freezeTicks > 0) return;
    if (m_hookState == HookState::Attached) return; // kancalıyken yerçekimini sik

    m_vel.y += GRAVITY * dt;
    m_vel.y = std::min(m_vel.y, MAX_FALL_SPEED);
}

void Player::updateHook(const Map& map, const PlayerInput& input, float dt) {
    if (m_freezeTicks > 0 || !input.hookHeld) {
        m_hookState = HookState::Idle;
        return;
    }

    if (m_hookState == HookState::Idle) {
        m_hookState = HookState::Flying;
        m_hookDir = input.aimDir.length() > 0.0001f ? input.aimDir.normalized() : Vec2{1.0f, 0.0f};
        m_hookPos = m_pos;
        m_hookTraveled = 0.0f;
    }

    if (m_hookState == HookState::Flying) {
        float travelThisTick = HOOK_FIRE_SPEED * dt;
        int samples = std::max(1, static_cast<int>(travelThisTick / HOOK_STEP));
        Vec2 sampleStep = m_hookDir * (travelThisTick / samples);

        for (int i = 0; i < samples; i++) {
            Vec2 candidate = m_hookPos + sampleStep;
            int tx = static_cast<int>(std::floor(candidate.x));
            int ty = static_cast<int>(std::floor(candidate.y));

            if (isSolid(map, tx, ty)) {
                m_hookState = HookState::Attached;
                m_hookPos = candidate;
                return;
            }

            m_hookPos = candidate;
            m_hookTraveled += sampleStep.length();

            if (m_hookTraveled >= HOOK_MAX_LENGTH) {
                m_hookState = HookState::Idle; // karakterin osurma durumuna geçmesi
                return;
            }
        }
        return;
    }

    if (m_hookState == HookState::Attached) {
        Vec2 toHook = m_hookPos - m_pos;
        float dist = toHook.length();

        if (dist > HOOK_MIN_DIST) {
            Vec2 dir = toHook.normalized();
            float alongVel = m_vel.x * dir.x + m_vel.y * dir.y;

            if (alongVel < HOOK_DRAG_MAX_SPEED) {
                m_vel += dir * (HOOK_DRAG_ACCEL * dt);
            }
        }
    }
}

void Player::moveAndCollide(const Map& map, float dt) {
    if (m_freezeTicks > 0) {
        m_vel = {0.0f, 0.0f};
        return;
    }

    float newX = m_pos.x + m_vel.x * dt;
    if (boxCollides(map, {newX, m_pos.y})) {
        m_vel.x = 0.0f;
    } else {
        m_pos.x = newX;
    }

    float newY = m_pos.y + m_vel.y * dt;
    if (boxCollides(map, {m_pos.x, newY})) {
        if (m_vel.y > 0.0f) {
            m_grounded = true;
            m_jumpsLeft = MAX_JUMPS;
        }
        m_vel.y = 0.0f;
    } else {
        m_pos.y = newY;
        m_grounded = false;
    }
}

void Player::checkFreeze(const Map& map) {
    if (m_freezeTicks > 0) {
        m_freezeTicks--;
        return;
    }

    int tx = static_cast<int>(std::floor(m_pos.x));
    int ty = static_cast<int>(std::floor(m_pos.y));

    if (tx >= 0 && ty >= 0 && tx < Map::WIDTH && ty < Map::HEIGHT &&
        map.getTile(tx, ty) == Map::SDA) {
        m_freezeTicks = FREEZE_TICKS;
        m_vel = {0.0f, 0.0f};
        m_hookState = HookState::Idle;
    }
}

void Player::tick(const Map& map, const PlayerInput& input, float dt) {
    checkFreeze(map);
    applyMovement(input, dt);
    updateHook(map, input, dt);
    applyGravity(dt);
    moveAndCollide(map, dt);
}