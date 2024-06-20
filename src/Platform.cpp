#include "Platform.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Constants.h"

extern SDL_Renderer* gRenderer;
extern Camera camera;

Platform::Platform(int x, int y, int width, int height, const std::string& texturePath)
    : mRect({ x, y, width, height }), mTexturePath(texturePath) {
    mTexture = IMG_LoadTexture(gRenderer, mTexturePath.c_str());
}

void Platform::render(SDL_Renderer* renderer) {
    SDL_Rect renderRect = mRect;
    renderRect.y -= camera.y;
    SDL_RenderCopy(renderer, mTexture, NULL, &renderRect);
}

SDL_Rect Platform::getRect() const {
    return mRect;
}

const std::string& Platform::getTexturePath() const {
    return mTexturePath;
}
