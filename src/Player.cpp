#include "Player.h"
#include "Constants.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string> // Dodaj ten include

extern SDL_Renderer* gRenderer;

Player::Player(vector<Platform>& platforms, Mix_Chunk* jumpSound, const string& characterTexture)
    : mPlatforms(platforms), mVelocityX(0), mVelocityY(0), mAccelerationY(1), mJumpCount(0), mOnGround(true), mFacingLeft(true), mJumpSound(jumpSound) {
    mTexture = IMG_LoadTexture(gRenderer, ("pictures/" + characterTexture).c_str());
    if (!mTexture) {
        std::cerr << "Failed to load texture: " << IMG_GetError() << std::endl;
    }
    mRect.x = SCREEN_WIDTH / 2;
    mRect.y = SCREEN_HEIGHT / 2;
    mRect.w = 75;
    mRect.h = 100;
}

void Player::update() {
    mVelocityY += mAccelerationY;

    // AKTUALIZUJEMY POZYCJE POSTACI NA PODSTAWIE PRĘDKOŚCI
    mRect.x += mVelocityX;
    mRect.y += mVelocityY;

    // KOLIZJA Z PLATFORMAMI
    mOnGround = false;
    for (auto& platform : mPlatforms) {
        SDL_Rect platformRect = platform.getRect();
        if (SDL_HasIntersection(&mRect, &platformRect)) {
            if (mRect.y + mRect.h - mVelocityY <= platformRect.y) {
                // LĄDOWANIE NA PLATFORMIE
                mRect.y = platformRect.y - mRect.h;
                mVelocityY = 0;
                mJumpCount = 0;
                mOnGround = true;
            }
        }
    }
}

void Player::render(SDL_Renderer* renderer) {
    SDL_RendererFlip flip = mFacingLeft ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_RenderCopyEx(renderer, mTexture, NULL, &mRect, 0, NULL, flip);
}

void Player::jump() {
    if (mOnGround || mJumpCount < 2) {
        mVelocityY = -15;
        mJumpCount++;
        mOnGround = false;
        Mix_PlayChannel(-1, mJumpSound, 0); // DŹWIĘK SKOKU
    }
}

void Player::moveLeft() {
    mVelocityX = -5;
    mFacingLeft = true;
}

void Player::moveRight() {
    mVelocityX = 5;
    mFacingLeft = false;
}

void Player::stop() {
    mVelocityX = 0;
}

int Player::getVelocityX() const {
    return mVelocityX;
}

SDL_Rect Player::getRect() const {
    return mRect;
}
