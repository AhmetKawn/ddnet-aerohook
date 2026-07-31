#pragma once

#include "../core/map.h"
#include "../core/player.h"

class Engine {
public:
    static constexpr float TICK_RATE = 50.0f;
    static constexpr float TICK_DT = 1.0f / TICK_RATE;

    explicit Engine(const Map& map);

    void setKeyState(bool left, bool right, bool jumpHeld, bool hookHeld);
    void setAimDirection(Vec2 aimDir);

    void update(float realDt);

    const Player& player() const { return m_player; }

private:
    void tick();

    const Map& m_map;
    Player m_player;

    bool m_left{false};
    bool m_right{false};
    bool m_jumpHeld{false};
    bool m_prevJumpHeld{false};
    bool m_hookHeld{false};
    Vec2 m_aimDir{1.0f, 0.0f};

    float m_accumulator{0.0f};
};