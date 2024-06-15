#ifndef PLATFORM_H
#define PLATFORM_H

#include <SDL2/SDL.h>

class Platform {
public:
    Platform(int x, int y, int width, int height);
    void render(SDL_Renderer* renderer);
    SDL_Rect getRect();

private:
    SDL_Rect mRect;
};

#endif
