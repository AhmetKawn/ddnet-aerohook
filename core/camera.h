#pragma once

#include "player.h" // Vec2

class Camera {
public:
    // Ekranda 1 tile kaç piksel görünsün. Harita artık ekrana sığdırılmıyor,
    // bunun yerine sabit bir zoom kullanılıyor (karakter küçük kalmasın diye).
    static constexpr float TILE_PIXEL_SIZE = 48.0f;

    // Kameranın hedefe her frame ne kadar yaklaşacağını belirler (0-1 arası).
    // Küçük değer = daha "elastik"/gecikmeli takip, büyük değer = daha sert kilitlenme.
    static constexpr float SMOOTHING = 0.15f;

    Camera();

    // Kamerayı doğrudan bir noktaya ışınlar (spawn anında yumuşak kaymayı önlemek için).
    void snapTo(Vec2 worldPos);

    // Kamerayı hedefe doğru yumuşakça hareket ettirir. dt gerçek (render) süredir.
    void update(Vec2 targetPos, float dt);

    // Verilen ekran boyutuna göre world(tile) -> screen(pixel) dönüşümü için offset üretir.
    void computeOffset(int screenWidth, int screenHeight, float& offsetX, float& offsetY) const;

    Vec2 center() const { return m_center; }
    static float tileSize() { return TILE_PIXEL_SIZE; }

private:
    Vec2 m_center;
    Vec2 m_prevCenter; // ileride render-interpolasyonu için hazır tutuluyor
};
