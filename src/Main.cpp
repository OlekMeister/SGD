#define SDL_MAIN_HANDLED
#include <windows.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include "Player.h"
#include "Platform.h"
#include "Constants.h"

using namespace std;

void init();
void loadMedia();
void close();
void main_menu();
void game(const string& character);
void renderGameOverText(int score);
void character_selection();
void generatePlatforms(vector<Platform>& platforms, int& lastPlatformY, int screenHeight, int score);
void resetGame();

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gMenuBackground = NULL;
SDL_Texture* gGameBackground = NULL;
Mix_Music* gMenuMusic = NULL;
Mix_Music* gGameMusic = NULL;
Mix_Chunk* gJumpSound = NULL;
Mix_Chunk* gGameOverSound = NULL;
Camera camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

void init() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    gWindow = SDL_CreateWindow("Bunny World", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    int imgFlags = IMG_INIT_PNG;
    IMG_Init(imgFlags);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    TTF_Init();
}

SDL_Texture* loadTexture(string path) {
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface != NULL) {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

void loadMedia() {
    gMenuBackground = loadTexture("pictures/bunnyworld.png");
    gGameBackground = loadTexture("pictures/background_2.png");
    gMenuMusic = Mix_LoadMUS("sound/menu.mp3");
    gGameMusic = Mix_LoadMUS("sound/game.wav");
    gJumpSound = Mix_LoadWAV("sound/jump.wav");
    gGameOverSound = Mix_LoadWAV("sound/gameover.wav");
}

void drawTextWithOutline(const string& text, TTF_Font* font, SDL_Color textColor, SDL_Color outlineColor, int x, int y) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    SDL_Surface* outlineSurface = TTF_RenderText_Solid(font, text.c_str(), outlineColor);

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    SDL_Texture* outlineTexture = SDL_CreateTextureFromSurface(gRenderer, outlineSurface);

    SDL_Rect textRect = { x, y, textSurface->w, textSurface->h };
    SDL_Rect outlineRect = { x - 2, y - 2, outlineSurface->w, outlineSurface->h };

    SDL_RenderCopy(gRenderer, outlineTexture, NULL, &outlineRect);
    SDL_RenderCopy(gRenderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_FreeSurface(outlineSurface);
    SDL_DestroyTexture(textTexture);
    SDL_DestroyTexture(outlineTexture);
}

void main_menu() {
    TTF_Font* font = TTF_OpenFont("arial.ttf", 28);
    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Color outlineColor = { 0, 0, 0, 255 };

    Mix_PlayMusic(gMenuMusic, -1);

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_1:
                        character_selection();
                        break;
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                }
            }
        }

        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, gMenuBackground, NULL, NULL);

        drawTextWithOutline("1. Start Game", font, textColor, outlineColor, SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50);
        drawTextWithOutline("ESC. Quit", font, textColor, outlineColor, SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 10);

        SDL_RenderPresent(gRenderer);
    }

    Mix_HaltMusic();
    TTF_CloseFont(font);
}

void character_selection() {
    TTF_Font* font = TTF_OpenFont("arial.ttf", 28);
    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Color outlineColor = { 0, 0, 0, 255 };

    bool quit = false;
    SDL_Event e;
    string selectedCharacter = "";

    while (!quit && selectedCharacter == "") {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_1:
                        selectedCharacter = "bunny_otis.png";
                        break;
                    case SDLK_2:
                        selectedCharacter = "bunny_pyrka.png";
                        break;
                    case SDLK_3:
                        selectedCharacter = "bunny_osioł.png";
                        break;
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                }
            }
        }

        SDL_RenderClear(gRenderer);

        drawTextWithOutline("Choose your character:", font, textColor, outlineColor, SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 150);
        drawTextWithOutline("1. Otis", font, textColor, outlineColor, SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 50);
        drawTextWithOutline("2. Pyrka", font, textColor, outlineColor, SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 10);
        drawTextWithOutline("3. Osioł", font, textColor, outlineColor, SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 70);
        drawTextWithOutline("ESC. Back to Menu", font, textColor, outlineColor, SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 130);

        SDL_RenderPresent(gRenderer);
    }

    if (!quit) {
        resetGame();  // Resetowanie gry przed rozpoczęciem nowej gry
        game(selectedCharacter);
    }

    TTF_CloseFont(font);
}

void renderGameOverText(int score) {
    TTF_Font* font = TTF_OpenFont("arial.ttf", 72);
    SDL_Color textColor = { 255, 0, 0, 255 };
    SDL_Color outlineColor = { 0, 0, 0, 255 };

    drawTextWithOutline("Koniec gry", font, textColor, outlineColor, SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 150);
    drawTextWithOutline("Wynik: " + to_string(score), font, textColor, outlineColor, SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2);

    SDL_RenderPresent(gRenderer);

    TTF_CloseFont(font);
}

void generatePlatforms(vector<Platform>& platforms, int& lastPlatformY, int screenHeight, int score) {
    int maxJumpHeight = 200;
    int maxJumpWidth = 200;
    int platformSpacing = 150;

    while (lastPlatformY > camera.y - screenHeight) {
        int x = rand() % (SCREEN_WIDTH - 100);
        int y = lastPlatformY - platformSpacing;

        if (platforms.back().getRect().x + 100 + maxJumpWidth < x || x < platforms.back().getRect().x - maxJumpWidth) {
            x = platforms.back().getRect().x + (rand() % (2 * maxJumpWidth) - maxJumpWidth);
            if (x < 0) x = 0;
            if (x > SCREEN_WIDTH - 100) x = SCREEN_WIDTH - 100;
        }

        std::string texturePath = "pictures/stone.png";
        if (score >= 50) {
            texturePath = "pictures/swamp.png";
        } else if (score >= 20) {
            texturePath = "pictures/ice.png";
        }

        platforms.push_back(Platform(x, y, 150, 30, texturePath));
        lastPlatformY = y;
    }
}

void game(const string& character) {
    vector<Platform> platforms;
    int lastPlatformY = SCREEN_HEIGHT - 150;
    platforms.push_back(Platform(SCREEN_WIDTH / 2 - 50, lastPlatformY, 150, 30, "pictures/stone.png"));

    Player player(platforms, gJumpSound, character);
    bool quit = false;
    SDL_Event e;

    int backgroundX = 0;
    bool gameOver = false;
    Uint32 gameOverTime = 0;
    int score = 0;
    const int initialGameOverLine = SCREEN_HEIGHT;
    int lastPlatformIndex = -1;

    Mix_PlayMusic(gGameMusic, -1);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            if (!gameOver && e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_SPACE:
                        player.jump();
                        break;
                    case SDLK_LEFT:
                        player.moveLeft();
                        break;
                    case SDLK_RIGHT:
                        player.moveRight();
                        break;
                }
            }
            if (!gameOver && e.type == SDL_KEYUP) {
                switch (e.key.keysym.sym) {
                    case SDLK_LEFT:
                        if (player.getVelocityX() < 0) {
                            player.stop();
                        }
                        break;
                    case SDLK_RIGHT:
                        if (player.getVelocityX() > 0) {
                            player.stop();
                        }
                        break;
                }
            }
        }

        if (!gameOver) {
            player.update();

            if (player.getRect().y < camera.y + SCREEN_HEIGHT / 2) {
                camera.y = player.getRect().y - SCREEN_HEIGHT / 2;
            }

            int gameOverLine = camera.y + SCREEN_HEIGHT * 1.5;

            if (player.getRect().y > gameOverLine) {
                Mix_HaltMusic();
                Mix_PlayChannel(-1, gGameOverSound, 0);
                gameOver = true;
                gameOverTime = SDL_GetTicks();
            }

            generatePlatforms(platforms, lastPlatformY, SCREEN_HEIGHT, score);

            for (size_t i = 0; i < platforms.size(); ++i) {
                if (player.getRect().y + player.getRect().h == platforms[i].getRect().y && static_cast<int>(i) > lastPlatformIndex) {
                    score++;
                    lastPlatformIndex = static_cast<int>(i);
                }
            }

            backgroundX -= player.getVelocityX() / 2;

            if (backgroundX <= -SCREEN_WIDTH) {
                backgroundX += SCREEN_WIDTH;
            }
            if (backgroundX >= SCREEN_WIDTH) {
                backgroundX -= SCREEN_WIDTH;
            }

            SDL_Rect bgRect1 = {backgroundX, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_Rect bgRect2 = {backgroundX - SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_Rect bgRect3 = {backgroundX + SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

            SDL_RenderClear(gRenderer);
            SDL_RenderCopy(gRenderer, gGameBackground, NULL, &bgRect1);
            SDL_RenderCopy(gRenderer, gGameBackground, NULL, &bgRect2);
            SDL_RenderCopy(gRenderer, gGameBackground, NULL, &bgRect3);

            player.render(gRenderer);
            for (auto& platform : platforms) {
                platform.render(gRenderer);
            }

            SDL_RenderPresent(gRenderer);
            SDL_Delay(16);
        } else {
            if (SDL_GetTicks() - gameOverTime > 1000) {
                renderGameOverText(score);
                while (Mix_Playing(-1) != 0) {
                    SDL_Delay(100);
                }
                quit = true;
            }
        }
    }

    Mix_HaltMusic();
    Mix_PlayMusic(gMenuMusic, -1);
}

void resetGame() {
    camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
}

void close() {
    SDL_DestroyTexture(gMenuBackground);
    SDL_DestroyTexture(gGameBackground);
    gMenuBackground = NULL;
    gGameBackground = NULL;
    Mix_FreeChunk(gJumpSound);
    gJumpSound = NULL;
    Mix_FreeChunk(gGameOverSound);
    gGameOverSound = NULL;
    Mix_FreeMusic(gMenuMusic);
    gMenuMusic = NULL;
    Mix_FreeMusic(gGameMusic);
    gGameMusic = NULL;
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;
    IMG_Quit();
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}

int main(int argc, char* args[]) {
    init();
    loadMedia();
    Mix_PlayMusic(gMenuMusic, -1);

    main_menu();

    close();
    return 0;
}
