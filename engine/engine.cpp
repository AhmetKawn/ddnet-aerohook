#include "engine.h"

Engine::Engine(const Map& map) : m_map(map) {
    m_player.spawn({2.0f, 10.0f}); // haritadaki 'S' spawn tile'ı (x=2, y=10)
}

void Engine::setKeyState(bool left, bool right, bool jumpHeld, bool hookHeld) {
    m_left = left;
    m_right = right;
    m_jumpHeld = jumpHeld;
    m_hookHeld = hookHeld;
}

void Engine::setAimDirection(Vec2 aimDir) {
    m_aimDir = aimDir;
}

void Engine::update(float realDt) {
    m_accumulator += realDt;

    while (m_accumulator >= TICK_DT) {
        tick();
        m_accumulator -= TICK_DT;
    }
}

void Engine::tick() {
    PlayerInput input;
    input.left = m_left;
    input.right = m_right;
    input.jumpHeld = m_jumpHeld;
    input.jumpPressed = m_jumpHeld && !m_prevJumpHeld;
    input.hookHeld = m_hookHeld;
    input.aimDir = m_aimDir;

    m_player.tick(m_map, input, TICK_DT);

    m_prevJumpHeld = m_jumpHeld;
}