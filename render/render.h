#pragma once

#include "../core/map.h"

class Renderer
{
public:
    Renderer() = default;
    ~Renderer();

    bool init(const char* spawnTexturePath);
    void cleanup();

    void drawMap(const Map& map, int width, int height);

private:
    bool loadTexture(const char* texturePath, unsigned int &texture, int &width, int &height,
                     bool clamp = false, bool nearest = false,
                     float* u0 = nullptr, float* v0 = nullptr,
                     float* u1 = nullptr, float* v1 = nullptr,
                     int* cropWidth = nullptr, int* cropHeight = nullptr);
    void drawTile(int x, int y, char tile, float tileSize, float offsetX, float offsetY);
    void drawTexturedTile(float px, float py, float size, unsigned int texture,
                          float u0 = 0.0f, float v0 = 0.0f, float u1 = 1.0f, float v1 = 1.0f,
                          bool flipV = false);

    unsigned int m_spawnTexture{0};
    int m_spawnTextureWidth{0};
    int m_spawnTextureHeight{0};
    float m_spawnU0{0.0f};
    float m_spawnV0{0.0f};
    float m_spawnU1{1.0f};
    float m_spawnV1{1.0f};
};
