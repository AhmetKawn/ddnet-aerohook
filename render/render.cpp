#include "render.h"

void Renderer::drawMap(const Map& map)
{
    for (int y = 0; y < Map::HEIGHT; y++)
    {
        for (int x = 0; x < Map::WIDTH; x++)
        {
            drawTile(x, y, map.getTile(x, y));
        }
    }
}

void Renderer::drawTile(int x, int y, char tile)
{
    switch (tile)
    {
        case Map::WALL:
            break;

        case Map::SDA:
            break;

        case Map::AIR:
            break;
    }
}