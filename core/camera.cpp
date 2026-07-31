#include "camera.h"

#include <cmath>

Camera::Camera()
    : m_center(0.0f, 0.0f), m_prevCenter(0.0f, 0.0f)
{
}

void Camera::snapTo(Vec2 worldPos)
{
    m_center = worldPos;
    m_prevCenter = worldPos;
}

void Camera::update(Vec2 targetPos, float dt)
{
    m_prevCenter = m_center;

    float t = 1.0f - std::pow(1.0f - SMOOTHING, dt * 60.0f);
    m_center.x += (targetPos.x - m_center.x) * t;
    m_center.y += (targetPos.y - m_center.y) * t;
}

void Camera::computeOffset(int screenWidth, int screenHeight, float& offsetX, float& offsetY) const
{
    offsetX = screenWidth * 0.5f - m_center.x * TILE_PIXEL_SIZE;
    offsetY = screenHeight * 0.5f - m_center.y * TILE_PIXEL_SIZE;
}