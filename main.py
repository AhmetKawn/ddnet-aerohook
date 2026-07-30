import sys
import pygame
from core.player import Player

def build_level(level_data, tile_size):
    tiles = []
    for y, row in enumerate(level_data):
        for x, col in enumerate(row):
            if col == 'X':
                tiles.append(pygame.Rect(x * tile_size, y * tile_size, tile_size, tile_size))
    return tiles

def main():
    pygame.init()
    WIDTH, HEIGHT = 800, 600
    screen = pygame.display.set_mode((WIDTH, HEIGHT))
    pygame.display.set_caption("AeroHook v2.0 - Proje Altyapisi")
    clock = pygame.time.Clock()

    level_map = [
        "XXXXXXXXXXXXXXXXXXXXXXXXX",
        "X                       X",
        "X       XXX             X",
        "X               XXXX    X",
        "X    XXXX               X",
        "X           XXX         X",
        "X                       X",
        "X   XXX                 X",
        "X                       X",
        "XXXXXXXXXXXXXXXXXXXXXXXXX"
    ]
    
    TILE_SIZE = 32
    tiles = build_level(level_map, TILE_SIZE)
    player = Player(100, 100)

    running = True
    while running:
        screen.fill((30, 35, 45))

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.MOUSEBUTTONDOWN:
                if event.button == 1:
                    mx, my = pygame.mouse.get_pos()
                    player.shoot_hook(mx, my, tiles)
            elif event.type == pygame.MOUSEBUTTONUP:
                if event.button == 1:
                    player.release_hook()
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE or event.key == pygame.K_UP or event.key == pygame.K_w:
                    player.jump()

        keys = pygame.key.get_pressed()
        if keys[pygame.K_a] or keys[pygame.K_LEFT]:
            player.vx -= 1.5
        if keys[pygame.K_d] or keys[pygame.K_RIGHT]:
            player.vx += 1.5

        player.update(tiles)

        for tile in tiles:
            pygame.draw.rect(screen, (80, 90, 110), tile)
            pygame.draw.rect(screen, (50, 60, 80), tile, 2)

        if player.hooked and player.hook_pos:
            pygame.draw.line(screen, (220, 220, 220), player.rect.center, player.hook_pos, 3)
            pygame.draw.circle(screen, (255, 80, 80), (int(player.hook_pos[0]), int(player.hook_pos[1])), 5)

        pygame.draw.rect(screen, (100, 200, 255), player.rect, border_radius=5)
        pygame.draw.circle(screen, (255, 255, 255), player.rect.center, 4)

        pygame.display.flip()
        clock.tick(60)

    pygame.quit()
    sys.exit()

if __name__ == "__main__":
    main()