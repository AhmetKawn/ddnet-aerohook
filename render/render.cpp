#include "render.h"

#include <algorithm>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/gl.h>
#endif

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

    for (int y = 0; y < Map::HEIGHT; y++)
    {
        for (int x = 0; x < Map::WIDTH; x++)
        {
            drawTile(x, y, map.getTile(x, y), tileSize, offsetX, offsetY);
        }
    }
}

void Renderer::drawTile(int x, int y, char tile, float tileSize, float offsetX, float offsetY)
{
    const float px = offsetX + x * tileSize;
    const float py = offsetY + y * tileSize;

    switch (tile)
    {
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