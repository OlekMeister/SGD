#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Player.h"
#include "Platform.h"
#include "Constants.h"

using namespace std;

void init();
void loadMedia();
void close();
void main_menu();
void game();
void renderGameOverText();

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gMenuBackground = NULL;
SDL_Texture* gGameBackground = NULL;
Mix_Music* gMenuMusic = NULL;
Mix_Music* gGameMusic = NULL;
Mix_Chunk* gJumpSound = NULL;
Mix_Chunk* gGameOverSound = NULL;

void init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        exit(1);
    }

    gWindow = SDL_CreateWindow("Bunny World", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        exit(1);
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == NULL) {
        exit(1);
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        exit(1);
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        exit(1);
    }

    if (TTF_Init() == -1) {
        exit(1);
    }
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
    gGameBackground = loadTexture("pictures/background.jpeg");
    gMenuMusic = Mix_LoadMUS("sound/menu.mp3");
    gGameMusic = Mix_LoadMUS("sound/game.wav");
    gJumpSound = Mix_LoadWAV("sound/jump.wav");
    gGameOverSound = Mix_LoadWAV("sound/gameover.wav");

    if (!gMenuBackground || !gGameBackground || !gMenuMusic || !gGameMusic || !gJumpSound || !gGameOverSound) {
        exit(1);
    }
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
                        game();
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

void renderGameOverText() {
    TTF_Font* font = TTF_OpenFont("arial.ttf", 72);
    SDL_Color textColor = { 255, 0, 0, 255 };
    SDL_Color outlineColor = { 0, 0, 0, 255 };

    drawTextWithOutline("Koniec gry", font, textColor, outlineColor, SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 50);
    SDL_RenderPresent(gRenderer);

    TTF_CloseFont(font);
}

void game() {
    vector<Platform> platforms;
    platforms.push_back(Platform(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT - 50, 100, 10));

    Player player(platforms, gJumpSound);
    bool quit = false;
    SDL_Event e;

    int backgroundX = 0;
    bool gameOver = false;
    Uint32 gameOverTime = 0;

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

            // CZY POSTAĆ SPADŁA ZA EKRAN
            if (player.getRect().y > SCREEN_HEIGHT) {
                Mix_HaltMusic();
                Mix_PlayChannel(-1, gGameOverSound, 0);
                gameOver = true;
                gameOverTime = SDL_GetTicks();
            }

            // AKTUALIZOWANIE TŁA
            backgroundX -= player.getVelocityX() / 2;
            if (backgroundX <= -SCREEN_WIDTH) {
                backgroundX += SCREEN_WIDTH;
            }
            if (backgroundX >= SCREEN_WIDTH) {
                backgroundX -= SCREEN_WIDTH;
            }

            SDL_Rect bgRect1 = { backgroundX, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
            SDL_Rect bgRect2 = { backgroundX - SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
            SDL_Rect bgRect3 = { backgroundX + SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

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
            // KONIEC GRY
            if (SDL_GetTicks() - gameOverTime > 1000) {
                renderGameOverText();
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
