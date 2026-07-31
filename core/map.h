#pragma once

#include <array>
#include <utility>

class Map {
public:
    static constexpr int WIDTH = 60;
    static constexpr int HEIGHT = 30;

    // Harita Blok Tipleri
    static constexpr char WALL = 'W';        // Katı duvar / Zemin
    static constexpr char AIR = '.';         // Boşluk / Hava
    static constexpr char SDA = 'F';         // Sigma Dondurucu Alan
    static constexpr char SPAWN = 'S';       // Başlangıç Spawn Noktası
    static constexpr char SPIKE = 'X';       // Ölümcül Dikenler
    static constexpr char BOOST = 'B';       // Süper Hızlandırıcı Pad
    static constexpr char CHECKPOINT = 'C';  // Ara Kontrol Noktası
    static constexpr char FINISH = 'E';      // Bitiş Çizgisi

    Map()
    {
        // 60x30 boyutunda, her satırı tam 60 karakterlik devasa DDNet parkuru
        m_tiles = {{
            "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
            "W..........................................................W",
            "W....WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW....W",
            "W..........................................................W",
            "W.......WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW.......W",
            "W..........................................................W",
            "W.S......................................................E.W",
            "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
            "W..................WW......................................W",
            "W...WWWWWWWW....WWWWWW....WWWWWWWW....WWWWWWWWWW....WW.....W",
            "W..........................................................W",
            "W......B........................B...................B......W",
            "WWWWWWWWWWWW....WWWWWW....WWWWWWWWWWWW....WWWWWWWWWWWWWWWW",
            "W..........................................................W",
            "W..........................................................W",
            "W....XXXXXXXXXXXXX....XXXXXXXXXXXXX....XXXXXXXXXXXXX.......W",
            "W....FFFFFFFFFFFFF....FFFFFFFFFFFFF....FFFFFFFFFFFFF.......W",
            "W....FFFFFFFFFFFFF....FFFFFFFFFFFFF....FFFFFFFFFFFFF.......W",
            "W....FFFFFFFFFFFFF....FFFFFFFFFFFFF....FFFFFFFFFFFFF.......W",
            "W..........................................................W",
            "W..........................................................W",
            "W..........C..............C..............C.................W",
            "W.....WWWWWWWWWW....WWWWWWWWWW....WWWWWWWWWW..............W",
            "W..........................................................W",
            "W..........................................................W",
            "W..........................................................W",
            "W..........................................................W",
            "W..........................................................W",
            "W..........................................................W",
            "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
        }};
    }

    // Güvenli blok okuma fonksiyonu (Harita dışına çıkmayı engeller)
    char getTile(int x, int y) const 
    {
        if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
            return WALL; 
        }
        return m_tiles[y][x];
    }

    // Pratik kontrol fonksiyonları
    bool isSolid(int x, int y) const {
        char t = getTile(x, y);
        return t == WALL;
    }

    bool isDeadly(int x, int y) const {
        char t = getTile(x, y);
        return t == SPIKE || t == SDA;
    }

    // Spawn noktasını otomatik bulmak için yardımcı
    std::pair<int, int> getSpawnPosition() const {
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                if (m_tiles[y][x] == SPAWN) {
                    return {x, y};
                }
            }
        }
        return {2, 6}; // Varsayılan yedek konum
    }

private:
    std::array<const char*, HEIGHT> m_tiles;
};
