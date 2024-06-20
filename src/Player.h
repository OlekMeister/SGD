#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <string>
#include "Platform.h"

class Player {
public:
    Player(std::vector<Platform>& platforms, Mix_Chunk* jumpSound, const std::string& characterTexture);
    void update();
    void render(SDL_Renderer* renderer);
    void jump();
    void moveLeft();
    void moveRight();
    void stop();
    int getVelocityX() const;
    SDL_Rect getRect() const;
    void setSliding(bool sliding);

private:
    SDL_Texture* mTexture;
    SDL_Rect mRect;
    int mVelocityX;
    int mVelocityY;
    int mAccelerationY;
    std::vector<Platform>& mPlatforms;
    int mJumpCount;
    bool mOnGround;
    bool mFacingLeft;
    bool mSliding;
    Mix_Chunk* mJumpSound;
    std::string mTexturePath;
};

#endif
