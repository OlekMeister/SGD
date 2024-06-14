import pygame
import sys
import random


pygame.init()

# ROZDZIELCZOŚĆ EKRANU GRY
width, height = 1024, 1024
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption('Bunny World')

# USTAWIENIA CZCIONEK
font = pygame.font.SysFont(None, 55)
menu_font = pygame.font.SysFont(None, 75)
outline_font = pygame.font.SysFont(None, 77)  # Czcionka do obramówki

# KOLORY (MENU)
WHITE = (255, 255, 255)
PURPLE = (128, 0, 128)
LIGHT_PURPLE = (186, 85, 211)
DARK_PURPLE = (75, 0, 130)

# TŁO MENU
menu_background = pygame.image.load('pictures/bunnyworld.png').convert_alpha()
menu_background = pygame.transform.scale(menu_background, (width, height))

# BACKGROUND W GRZE
game_background = pygame.image.load('pictures/background.png').convert_alpha()
game_background_width = game_background.get_width()

# WSZYSTKIE DŹWIĘKI ZNAJDUJĄCE SIĘ W GRZE
pygame.mixer.music.load('sound/Caketown.mp3')
jump_sound = pygame.mixer.Sound('sound/jump.wav')

# OBRAZEK POSTACI, POMNIEJSZANY
bunny_image = pygame.image.load('pictures/bunny_transparent.png').convert_alpha()
bunny_image = pygame.transform.scale(bunny_image, (75, 100))  # Zmniejszenie obrazu królika

# GRACZ
class Player(pygame.sprite.Sprite):
    def __init__(self, platforms):
        super().__init__()
        self.image = bunny_image
        self.original_image = self.image
        self.rect = self.image.get_rect()
        self.rect.center = (width // 2, height // 2)
        self.velocity_x = 0
        self.velocity_y = 0
        self.acceleration_y = 1
        self.platforms = platforms
        self.jump_count = 0
        self.facing_left = True

    def update(self):
        # RUCH W POZIOMIE
        keys = pygame.key.get_pressed()
        self.velocity_x = 0
        if keys[pygame.K_LEFT]:
            self.velocity_x = -5
            if not self.facing_left:
                self.image = self.original_image
                self.facing_left = True
        elif keys[pygame.K_RIGHT]:
            self.velocity_x = 5
            if self.facing_left:
                self.image = pygame.transform.flip(self.original_image, True, False)
                self.facing_left = False

        # RUCH W PIONIE
        self.velocity_y += self.acceleration_y

        # AKTUALIZACJA POZYCJI
        self.rect.x += self.velocity_x
        self.rect.y += self.velocity_y

        # KOLIZJA Z PLATFORMĄ
        hits = pygame.sprite.spritecollide(self, self.platforms, False)
        if hits:
            self.rect.y = hits[0].rect.top - self.rect.height
            self.velocity_y = 0
            self.jump_count = 0  # POSTAĆ POSIADA MAX 2 SKOKI, TUTAJ RESETUJEMY LICZBE SKOKÓW PRZY LĄDOWANIU

    def jump(self):
        if self.jump_count < 2:  #CZY GRACZ MOŻE SKOCZYĆ
            self.velocity_y = -15
            self.jump_count += 1
            jump_sound.play()

# PLATFORMA
class Platform(pygame.sprite.Sprite):
    def __init__(self, x, y, width, height):
        super().__init__()
        self.image = pygame.Surface((width, height))
        self.image.fill((255, 255, 255))
        self.rect = self.image.get_rect()
        self.rect.x = x
        self.rect.y = y

def draw_text_with_outline(text, font, outline_font, text_color, outline_color, surface, x, y):
    #OBRAMÓWKA BIAŁA DO MENU
    outline = outline_font.render(text, True, outline_color)
    text_surface = font.render(text, True, text_color)

    outline_rect = outline.get_rect(center=(x, y))
    text_rect = text_surface.get_rect(center=(x, y))

    surface.blit(outline, outline_rect.move(-2, 0))
    surface.blit(outline, outline_rect.move(2, 0))
    surface.blit(outline, outline_rect.move(0, -2))
    surface.blit(outline, outline_rect.move(0, 2))

    surface.blit(text_surface, text_rect)

def generate_platforms(platforms):
    # GENEROWANIE LOSOWO PLATFORM (NA RAZIE NIE DZIAŁA NW CZEMU)
    while len(platforms) < 10:
        last_platform = platforms.sprites()[-1]
        new_x = last_platform.rect.x + random.randint(200, 400)
        new_y = random.randint(height // 2, height - 50)
        new_platform = Platform(new_x, new_y, 100, 10)
        platforms.add(new_platform)

def main_menu():
    selected_option = 0
    options = ['Rozpocznij grę', 'Wyjście']  # MENU OPCJE

    while True:
        screen.blit(menu_background, (0, 0))  # TŁO MENU

        for i, option in enumerate(options):
            y_pos = height - 100 - (len(options) - i - 1) * 100
            color = LIGHT_PURPLE if i == selected_option else DARK_PURPLE
            draw_text_with_outline(option, menu_font, outline_font, color, WHITE, screen, width // 2, y_pos)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_DOWN:
                    selected_option = (selected_option + 1) % len(options)
                if event.key == pygame.K_UP:
                    selected_option = (selected_option - 1) % len(options)
                if event.key == pygame.K_RETURN:
                    if selected_option == 0:
                        game()
                    elif selected_option == 1:
                        pygame.quit()
                        sys.exit()

        pygame.display.flip()
        pygame.time.Clock().tick(60)

def game():
    platforms = pygame.sprite.Group()
    platform = Platform(width // 2 - 50, height - 50, 100, 10)
    platforms.add(platform)

    player = Player(platforms)
    all_sprites = pygame.sprite.Group()
    all_sprites.add(player)
    all_sprites.add(platform)

    background_x = 0
    background_width = game_background_width
    # SZYBKOŚĆ PRZEWIJANIA TŁA
    background_scroll_factor = 0.5

    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    player.jump()

        all_sprites.update()
        generate_platforms(platforms)

        # TŁO PRZESUWA SIĘ RAZEM Z POSTACIĄ
        if player.velocity_x != 0:
            background_x -= player.velocity_x * background_scroll_factor
            if background_x <= -background_width:
                background_x = 0
            elif background_x >= background_width:
                background_x = 0

        # TŁO
        screen.blit(game_background, (background_x, 0))
        screen.blit(game_background, (background_x + background_width, 0))
        all_sprites.draw(screen)

        pygame.display.flip()
        pygame.time.Clock().tick(60)

    pygame.quit()
    sys.exit()

# MUZYKA W TLE (-1 POWODUJE PĘTLE)
pygame.mixer.music.play(-1)

main_menu()
