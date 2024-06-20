#ifndef PLATFORM_H
#define PLATFORM_H

#include <SDL2/SDL.h>
#include <string>

class Platform {
public:
    Platform(int x, int y, int width, int height, const std::string& texturePath);
    void render(SDL_Renderer* renderer);
    SDL_Rect getRect() const;
    const std::string& getTexturePath() const;

private:
    SDL_Rect mRect;
    SDL_Texture* mTexture;
    std::string mTexturePath;
};

#endif
