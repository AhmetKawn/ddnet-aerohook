#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "render.h"

#include <algorithm>
#include <iostream>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/gl.h>
#endif

Renderer::~Renderer()
{
    cleanup();
}

bool Renderer::init(const char* spawnTexturePath)
{
    if (!loadTexture(spawnTexturePath, m_spawnTexture, m_spawnTextureWidth, m_spawnTextureHeight,
                     true, true)) {
        std::cerr << "Failed to initialize spawn texture." << std::endl;
        cleanup();
        return false;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return true;
}

bool Renderer::loadTexture(const char* texturePath, unsigned int &texture, int &width, int &height,
                           bool clamp, bool nearest,
                           float* u0, float* v0,
                           float* u1, float* v1,
                           int* cropWidth, int* cropHeight)
{
    int channels = 0;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(texturePath, &width, &height, &channels, STBI_rgb_alpha);
    if (!data) {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
        return false;
    }

    if (u0 && v0 && u1 && v1) {
        int minX = width;
        int minY = height;
        int maxX = -1;
        int maxY = -1;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                unsigned char alpha = data[(y * width + x) * 4 + 3];
                if (alpha != 0) {
                    minX = std::min(minX, x);
                    minY = std::min(minY, y);
                    maxX = std::max(maxX, x);
                    maxY = std::max(maxY, y);
                }
            }
        }

        if (maxX >= minX && maxY >= minY) {
            if (cropWidth) *cropWidth = maxX - minX + 1;
            if (cropHeight) *cropHeight = maxY - minY + 1;
            *u0 = minX / static_cast<float>(width);
            *v0 = minY / static_cast<float>(height);
            *u1 = (maxX + 1) / static_cast<float>(width);
            *v1 = (maxY + 1) / static_cast<float>(height);
        } else {
            if (cropWidth) *cropWidth = width;
            if (cropHeight) *cropHeight = height;
            *u0 = 0.0f;
            *v0 = 0.0f;
            *u1 = 1.0f;
            *v1 = 1.0f;
        }
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, nearest ? GL_NEAREST : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, nearest ? GL_NEAREST : GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
    return true;
}

void Renderer::cleanup()
{
    if (m_spawnTexture != 0) {
        glDeleteTextures(1, &m_spawnTexture);
        m_spawnTexture = 0;
    }
}

void Renderer::drawMap(const Map& map, int width, int height)
{
    const float tileSize = std::min(static_cast<float>(width) / Map::WIDTH,
                                    static_cast<float>(height) / Map::HEIGHT);
    const float offsetX = (width - tileSize * Map::WIDTH) / 2.0f;
    const float offsetY = (height - tileSize * Map::HEIGHT) / 2.0f;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, static_cast<GLdouble>(width), static_cast<GLdouble>(height), 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for (int y = 0; y < Map::HEIGHT; y++) {
        for (int x = 0; x < Map::WIDTH; x++) {
            drawTile(x, y, map.getTile(x, y), tileSize, offsetX, offsetY);
        }
    }
}

void Renderer::drawTile(int x, int y, char tile, float tileSize, float offsetX, float offsetY)
{
    const float px = offsetX + x * tileSize;
    const float py = offsetY + y * tileSize;

    if (tile == Map::SPAWN && m_spawnTexture != 0) {
        float scale = 1.35f;
        float size = tileSize * scale;
        float delta = (size - tileSize) * 0.5f;
        drawTexturedTile(px - delta, py - delta, size, m_spawnTexture,
                         0.0f, 0.0f, 1.0f, 1.0f, false);
        return;
    }

    switch (tile) {
        case Map::WALL:
            glColor3f(0.35f, 0.35f, 0.35f);
            break;
        case Map::SDA:
            glColor3f(0.95f, 0.25f, 0.25f);
            break;
        case Map::AIR:
        default:
            glColor3f(0.10f, 0.10f, 0.10f);
            break;
    }

    glBegin(GL_QUADS);
    glVertex2f(px, py);
    glVertex2f(px + tileSize, py);
    glVertex2f(px + tileSize, py + tileSize);
    glVertex2f(px, py + tileSize);
    glEnd();
}

void Renderer::drawTexturedTile(float px, float py, float size, unsigned int texture,
                                float u0, float v0, float u1, float v1,
                                bool flipV)
{
    const float topV = flipV ? v1 : v0;
    const float bottomV = flipV ? v0 : v1;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    glTexCoord2f(u0, topV);
    glVertex2f(px, py);
    glTexCoord2f(u1, topV);
    glVertex2f(px + size, py);
    glTexCoord2f(u1, bottomV);
    glVertex2f(px + size, py + size);
    glTexCoord2f(u0, bottomV);
    glVertex2f(px, py + size);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}
