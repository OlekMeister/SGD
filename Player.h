#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include "Platform.h"

using namespace std;

class Player {
public:
    Player(vector<Platform>& platforms, Mix_Chunk* jumpSound); // DŹWIĘK SKOKU
    void update();
    void render(SDL_Renderer* renderer);
    void jump();
    void moveLeft();
    void moveRight();
    void stop();
    int getVelocityX() const;
    SDL_Rect getRect() const;

private:
    SDL_Texture* mTexture;
    SDL_Rect mRect;
    int mVelocityX;
    int mVelocityY;
    int mAccelerationY;
    vector<Platform>& mPlatforms;
    int mJumpCount;
    bool mOnGround;
    bool mFacingLeft; // W KTÓRĄ STRONE POSTAĆ JEST ZWRÓCONA
    Mix_Chunk* mJumpSound;
};

#endif
