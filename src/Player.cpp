#include "Player.h"
#include "Constants.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

extern SDL_Renderer* gRenderer;
extern Camera camera;

Player::Player(std::vector<Platform>& platforms, Mix_Chunk* jumpSound, const std::string& characterTexture)
    : mPlatforms(platforms), mVelocityX(0), mVelocityY(0), mAccelerationY(1), mJumpCount(0), mOnGround(true), mFacingLeft(true), mSliding(false), mJumpSound(jumpSound) {
    mTexture = IMG_LoadTexture(gRenderer, characterTexture.c_str());
    if (!mTexture) {
        std::cerr << "Failed to load texture: " << IMG_GetError() << std::endl;
    }
    mRect.x = SCREEN_WIDTH / 2;
    mRect.y = SCREEN_HEIGHT / 2;
    mRect.w = 75;
    mRect.h = 100;
    mTexturePath = "C:/Users/Aleksander/Desktop/SGD/pictures/stone.png";
}

void Player::update() {
    mVelocityY += mAccelerationY;

    mRect.x += mVelocityX;
    mRect.y += mVelocityY;

    mOnGround = false;
    for (auto& platform : mPlatforms) {
        SDL_Rect platformRect = platform.getRect();
        if (SDL_HasIntersection(&mRect, &platformRect)) {
            if (mRect.y + mRect.h - mVelocityY <= platformRect.y) {
                mRect.y = platformRect.y - mRect.h;
                mVelocityY = 0;
                mJumpCount = 0;
                mOnGround = true;
                mTexturePath = platform.getTexturePath();

                if (mTexturePath == "C:/Users/Aleksander/Desktop/SGD/pictures/swamp.png") {
                    mVelocityX = 0;
                    mSliding = false;
                } else if (mTexturePath == "C:/Users/Aleksander/Desktop/SGD/pictures/ice.png") {
                    mSliding = true;
                } else {
                    mSliding = false;
                }
                std::cout << "Landed on platform. TexturePath: " << mTexturePath << std::endl;
            }
        }
    }

    if (mSliding && mOnGround) {
        if (mVelocityX > 0) {
            mVelocityX -= 1;
            if (mVelocityX < 0) {
                mVelocityX = 0;
            }
        } else if (mVelocityX < 0) {
            mVelocityX += 1;
            if (mVelocityX > 0) {
                mVelocityX = 0;
            }
        }
        if (mVelocityX == 0) {
            mVelocityX = mFacingLeft ? -2 : 2;
        }
        std::cout << "On ice platform. VelocityX: " << mVelocityX << std::endl;
    }
}

void Player::render(SDL_Renderer* renderer) {
    SDL_Rect renderRect = mRect;
    renderRect.y -= camera.y;
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
    if (mOnGround && mTexturePath == "C:/Users/Aleksander/Desktop/SGD/pictures/swamp.png") {
        return;
    }
    if (mSliding) {
        if (mVelocityX > -10) {
            mVelocityX -= 5;
        }
    } else {
        mVelocityX = -5;
    }
    mFacingLeft = true;
}

void Player::moveRight() {
    if (mOnGround && mTexturePath == "C:/Users/Aleksander/Desktop/SGD/pictures/swamp.png") {
        return;
    }
    if (mSliding) {
        if (mVelocityX < 10) {
            mVelocityX += 5;
        }
    } else {
        mVelocityX = 5;
    }
    mFacingLeft = false;
}

void Player::stop() {
    if (!mSliding) {
        mVelocityX = 0;
    }
}

int Player::getVelocityX() const {
    return mVelocityX;
}

SDL_Rect Player::getRect() const {
    return mRect;
}

void Player::setSliding(bool sliding) {
    mSliding = sliding;
}
