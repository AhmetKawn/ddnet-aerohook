#pragma once

#include "../core/map.h"

class Renderer
{
public:
    void drawMap(const Map& map);

private:
    void drawTile(int x, int y, char tile);
};