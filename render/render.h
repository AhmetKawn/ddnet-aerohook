#pragma once

#include "../core/map.h"

class Renderer
{
public:
    void drawMap(const Map& map, int width, int height);

private:
    void drawTile(int x, int y, char tile, float tileSize, float offsetX, float offsetY);
};