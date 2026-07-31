#include "map.h"
#include <engine/map.h>
#include <engine/kernel.h>
#include <game/mapitems.h>
#include <game/client/gameclient.h>
#include <base/vmath.h>

// Constructor (Kurucu)
CGameMap::CGameMap()
{
    m_Width = 0;
    m_Height = 0;
    m_pTiles = nullptr;
}

void CGameMap::OnInit()
{
    // Motor üzerinden IMap arayüzünü çağır (harita verilerine erişim sağlar)
    IMap *pMap = Kernel()->RequestInterface<IMap>();
    if(!pMap)
        return;

    // Haritadaki objelerin listesinde gezinerek Çarpışma (Game) katmanını bul
    int Start, Num;
    pMap->GetType(MAPITEMTYPE_LAYER, &Start, &Num);

    for(int i = 0; i < Num; i++)
    {
        CMapItemLayer *pItem = (CMapItemLayer *)pMap->GetItem(Start + i, 0, 0);
        
        // Sadece Tilemap (Kare harita) tipindeki katmanları işle
        if(pItem && pItem->m_Type == LAYERTYPE_TILES)
        {
            CMapItemLayerTilemap *pTilemap = (CMapItemLayerTilemap *)pItem;
            
            // Çarpışma katmanı genellikle haritadaki özel bir flags veya isimlendirme ile ayrılır
            // Standart DDNet'te Game katmanı indeksleri üzerinden kontrol edilir.
            int Size = 0;
            CTile *pTiles = (CTile *)pMap->GetDataSwapped(pTilemap->m_Data, &Size);
            
            if(pTiles)
            {
                m_Width = pTilemap->m_Width;
                m_Height = pTilemap->m_Height;
                m_pTiles = pTiles;
                break; // Ana katmanı bulduğumuzda döngüden çık
            }
        }
    }
}

int CGameMap::GetCollisionAt(vec2 Pos)
{
    // Harita yüklenmediyse boş (0) döndür
    if(!m_pTiles)
        return 0; 

    // DDNet (Teeworlds) oyununda varsayılan karo (tile) boyutu 32x32 pikseldir.
    // Verilen X,Y pozisyonunu 32'ye bölerek harita gridindeki yerine (indeks) dönüştürüyoruz.
    int Nx = clamp(round_to_int(Pos.x) / 32, 0, m_Width - 1);
    int Ny = clamp(round_to_int(Pos.y) / 32, 0, m_Height - 1);

    // 2 Boyutlu koordinatı, 1 Boyutlu Array (Dizi) indeksine çevir
    int Index = Ny * m_Width + Nx;

    // Bulunan kareden çarpışma indeksini döndür (örn: TILE_SOLID, TILE_NOHOOK, TILE_DEATH)
    return m_pTiles[Index].m_Index;
}

void CGameMap::OnRender()
{
    // Tile tabanlı arka plan veya özel kaplamalar (sprite) çizilecekse buraya eklenir.
    // Standart harita çizimi CMapLayers class'ı tarafından arka planda halledilir erdem artık kodumuz 1 saatır deği babuş -ahmet kaan-.
}
