import math
import pygame

class Player:
    def __init__(self, x, y):
        self.rect = pygame.Rect(x, y, 26, 26) # 26x26 Hitbox
        self.vx = 0
        self.vy = 0
        self.gravity = 0.6
        self.friction = 0.85 # Havadayken ve yerdeyken kayma hissi
        
        # Kanca (Hook) Sistemleri
        self.hooked = False
        self.hook_pos = None
        self.max_hook_length = 300 # Maksimum ip uzunluğu
        
        self.on_ground = False

    def update(self, tiles):
        # 1. Kanca Fiziği (Sarkaç ve Çekme Kuvveti)
        if self.hooked and self.hook_pos:
            hx, hy = self.hook_pos
            dx = hx - self.rect.centerx
            dy = hy - self.rect.centery
            dist = math.hypot(dx, dy)
            
            if dist > 0:
                pull_force = 1.2 # İpin çekme gücü
                angle = math.atan2(dy, dx)
                self.vx += math.cos(angle) * pull_force
                self.vy += math.sin(angle) * pull_force
        
        # 2. Yerçekimi ve Sürtünme Uygulaması
        self.vy += self.gravity
        self.vx *= self.friction
        if not self.hooked:
            self.vy *= 0.98 # Hafif hava direnci
            
        # 3. Fizik Patlamalarını Önlemek İçin Hız Limiti (Terminal Velocity)
        self.vx = max(-18, min(18, self.vx))
        self.vy = max(-22, min(22, self.vy))

        # 4. X Ekseni Hareketi ve Çarpışma
        self.rect.x += int(self.vx)
        self.collide(tiles, x_axis=True)
        
        # 5. Y Ekseni Hareketi ve Çarpışma
        self.rect.y += int(self.vy)
        self.on_ground = False
        self.collide(tiles, x_axis=False)

    def collide(self, tiles, x_axis):
        """Oyuncunun haritadaki duvarlarla çarpışmasını (Hitbox) hesaplar."""
        for tile in tiles:
            if self.rect.colliderect(tile):
                if x_axis:
                    if self.vx > 0: # Sağa giderken duvara çarptı
                        self.rect.right = tile.left
                    elif self.vx < 0: # Sola giderken duvara çarptı
                        self.rect.left = tile.right
                    self.vx = 0
                else:
                    if self.vy > 0: # Düşerken zemine çarptı
                        self.rect.bottom = tile.top
                        self.on_ground = True
                    elif self.vy < 0: # Zıplarken tavana kafasını çarptı
                        self.rect.top = tile.bottom
                    self.vy = 0

    def jump(self):
        """Sadece yerdeyse zıplamaya izin ver (Jetpack hatası çözümü)."""
        if self.on_ground:
            self.vy = -12
            self.on_ground = False

    def shoot_hook(self, target_x, target_y, tiles):
        """Raycast algoritması: Kanca boşluğa mı yoksa duvara mı geldi hesaplar."""
        start_x, start_y = self.rect.center
        angle = math.atan2(target_y - start_y, target_x - start_x)
        
        step_size = 10
        current_distance = 0
        
        # Işını (ray) fareye doğru adım adım ilerlet
        while current_distance < self.max_hook_length:
            test_x = start_x + math.cos(angle) * current_distance
            test_y = start_y + math.sin(angle) * current_distance
            
            # Bu nokta bir duvarın içinde mi?
            hit_rect = pygame.Rect(test_x, test_y, 1, 1)
            if hit_rect.collidelist(tiles) != -1: 
                self.hooked = True
                self.hook_pos = (test_x, test_y)
                return # Duvara çarptı, tutun ve döngüden çık
            
            current_distance += step_size
            
        # Eğer döngü biterse ve duvara çarpmazsa, kanca boşa gider
        self.release_hook()

    def release_hook(self):
        self.hooked = False
        self.hook_pos = None
