#pragma once

#include <array>

class Map {
    public:
        static constexpr int WIDTH = 20;
        static constexpr int HEIGHT = 12;

        // şunları yazmak baya zevkli la
        static constexpr char WALL = 'W';
        static constexpr char AIR = '.'; 
        static constexpr char SDA = 'F'; // SDA sigma dondurucu alan anlamına geliyor

        Map()
        {
            m_tiles = {{ // bu array'ı elimle çizdim.
                "WWWWWWWWWWWWWWWWWWWW",
                "W..................W",
                "W..................W",
                "W..................W",
                "W..................W",
                "W..................W",
                "W..................W",
                "W..................W",
                "W..................W",
                "W..................W",
                "W..................W",
                "WWWWWWWWWWWWWWWWWWWW"
            }};
        }

        char getTile(int x, int y) const {
            return m_tiles[y][x];
        }

    private:
        std::array<const char*, HEIGHT> m_tiles;
    };