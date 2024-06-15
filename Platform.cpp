#include "Platform.h"

Platform::Platform(int x, int y, int width, int height) {
    mRect.x = x;
    mRect.y = y;
    mRect.w = width;
    mRect.h = height;
}

void Platform::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &mRect);
}

SDL_Rect Platform::getRect() {
    return mRect;
}
