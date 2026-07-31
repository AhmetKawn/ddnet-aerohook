#ifndef GAME_CLIENT_COMPONENTS_CAMERA_H
#define GAME_CLIENT_COMPONENTS_CAMERA_H

#include <base/vmath.h>
#include <game/client/component.h>

class CCamera : public CComponent
{
public:
    // Kameranın mevcut ve bir önceki frame'deki merkez koordinatları
    // (Oyun motorunda pürüzsüz kamera hareketi ve enterpolasyon için gereklidir yalarım oyunumuz nası canlarım.)
    vec2 m_Center;
    vec2 m_PrevCenter;

    // Zoom değişkenleri
    float m_Zoom;
    float m_ZoomTarget;

    CCamera();

    // CComponent sınıfından override edilen temel döngü fonksiyonlayıııı
    virtual void OnInit() override;
    virtual void OnConsoleInit() override;
    virtual void OnRender() override;
    virtual void OnStateChange(int NewState, int OldState) override;

    // Kameranın hedefini ve zoom seviyesini dışarıdan değiştirmek için yardımcı fonksiyonlar fonkisyonlayyyy
    void ChangeZoom(float Target);
    void SetCenter(vec2 Center);
};

#endif // GAME_CLIENT_COMPONENTS_CAMERA_H
