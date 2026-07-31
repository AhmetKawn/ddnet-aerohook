#pragma once

#include "map.h"

struct Vec2 {
    float x{0.0f};
    float y{0.0f};

    Vec2() = default;
    Vec2(float x_, float y_) : x(x_), y(y_) {}

    Vec2 operator+(const Vec2& o) const { return {x + o.x, y + o.y}; }
    Vec2 operator-(const Vec2& o) const { return {x - o.x, y - o.y}; }
    Vec2 operator*(float s) const { return {x * s, y * s}; }
    Vec2& operator+=(const Vec2& o) { x += o.x; y += o.y; return *this; }
    Vec2& operator*=(float s) { x *= s; y *= s; return *this; }

    float length() const;
    Vec2 normalized() const;
};

enum class HookState {
    Idle,
    Flying,
    Attached
};

struct PlayerInput {
    bool left{false};
    bool right{false};
    bool jumpHeld{false};      // tuş şu an basılı mı
    bool jumpPressed{false};   // bu tick'te yeni mi basıldı (edge-trigger, Engine hesaplıyor)
    bool hookHeld{false};      // fare sol tuşu basılı mı
    Vec2 aimDir{1.0f, 0.0f};   // fare yönü (normalize)
};

class Player {
public:
    static constexpr float RADIUS = 0.4f; // tile cinsinden yarı-genişlik (hitbox)

    Player();

    void spawn(Vec2 pos);
    void tick(const Map& map, const PlayerInput& input, float dt);

    Vec2 position() const { return m_pos; }
    Vec2 velocity() const { return m_vel; }
    bool isGrounded() const { return m_grounded; }
    bool isFrozen() const { return m_freezeTicks > 0; }

    HookState hookState() const { return m_hookState; }
    Vec2 hookPosition() const { return m_hookPos; }

private:
    void applyMovement(const PlayerInput& input, float dt);
    void applyGravity(float dt);
    void updateHook(const Map& map, const PlayerInput& input, float dt);
    void moveAndCollide(const Map& map, float dt);
    void checkFreeze(const Map& map);

    bool isSolid(const Map& map, int tx, int ty) const;
    bool boxCollides(const Map& map, Vec2 pos) const;

    Vec2 m_pos;
    Vec2 m_vel;
    bool m_grounded{false};
    int m_jumpsLeft{0};

    HookState m_hookState{HookState::Idle};
    Vec2 m_hookPos;
    Vec2 m_hookDir;
    float m_hookTraveled{0.0f};

    int m_freezeTicks{0};
};