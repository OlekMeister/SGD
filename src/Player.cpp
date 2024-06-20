#include "Player.h"
#include "Constants.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

extern SDL_Renderer* gRenderer;
extern Camera camera;

Player::Player(std::vector<Platform>& platforms, Mix_Chunk* jumpSound, const std::string& characterTexture)
    : mPlatforms(platforms), mVelocityX(0), mVelocityY(0), mAccelerationY(1), mJumpCount(0), mOnGround(true), mFacingLeft(true), mJumpSound(jumpSound) {
    mTexture = IMG_LoadTexture(gRenderer, ("pictures/" + characterTexture).c_str());
    if (!mTexture) {
        std::cerr << "Failed to load texture: " << IMG_GetError() << std::endl;
    }
    mRect.x = SCREEN_WIDTH / 2;
    mRect.y = SCREEN_HEIGHT / 2;
    mRect.w = 75;
    mRect.h = 100;
    mTexturePath = "pictures/stone.png";
}

void Player::update() {
    if (mTexturePath == "pictures/ice.png") {
        mVelocityX *= 0.98;
    }

    if (mTexturePath != "pictures/swamp.png") {
        mVelocityY += mAccelerationY;
    }

    mRect.x += mVelocityX;
    mRect.y += mVelocityY;

    mOnGround = false;
    for (auto& platform : mPlatforms) {
        SDL_Rect platformRect = platform.getRect();
        if (SDL_HasIntersection(&mRect, &platformRect)) {
            if (mRect.y + mRect.h - mVelocityY <= platformRect.y) {
                // Landing on platform
                mRect.y = platformRect.y - mRect.h;
                mVelocityY = 0;
                mJumpCount = 0;
                mOnGround = true;
                mTexturePath = platform.getTexturePath();

                if (mTexturePath == "pictures/swamp.png") {
                    mVelocityX = 0;
                }
            }
        }
    }
}

void Player::render(SDL_Renderer* renderer) {
    SDL_Rect renderRect = mRect;
    renderRect.y -= camera.y; // Adjust for camera offset
    SDL_RendererFlip flip = mFacingLeft ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_RenderCopyEx(renderer, mTexture, NULL, &renderRect, 0, NULL, flip);
}

void Player::jump() {
    if (mOnGround || mJumpCount < 2) {
        mVelocityY = -15;
        mJumpCount++;
        mOnGround = false;
        Mix_PlayChannel(-1, mJumpSound, 0);
    }
}

void Player::moveLeft() {
    if (mTexturePath != "pictures/swamp.png") {
        mVelocityX = -5;
        mFacingLeft = true;
    }
}

void Player::moveRight() {
    if (mTexturePath != "pictures/swamp.png") {
        mVelocityX = 5;
        mFacingLeft = false;
    }
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

const std::string& Player::getTexturePath() const {
    return mTexturePath;
}
