#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include<SDL2/SDL.h>
#include <string>
#include "../TextureManager.h"
#include "../AssetManager.h"
#include "../Animation.h"

class SpriteComponent : public Component {
    private:
    TransformComponent* transform;
    SDL_Texture* texture;
    SDL_Rect sourceRectangle;
    SDL_Rect destinationRectangle;
    bool isAnimated;
    int numFrame;
    int animationSpeed;
    bool isFixed;
    std::map<std::string, Animation> animations;
    std::string currentAnimationName;
    unsigned int animationIndex = 0;

    public:
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    SpriteComponent(std::string assetTextureId){
        isAnimated = false;
        isFixed = false;
        SetTexture(assetTextureId);
    }

    SpriteComponent(std::string id, int numFrame, int animationSpeed, bool hasDirection, bool isFixed){
        this->isAnimated = true;
        this->numFrame = numFrame;
        this->animationSpeed = animationSpeed;
        this->isFixed = isFixed;

        if(hasDirection){
            Animation downAnimation = Animation(0, numFrame, animationSpeed);
            Animation rightAnimation = Animation(1, numFrame, animationSpeed);
            Animation leftAnimation = Animation(2, numFrame, animationSpeed);
            Animation upAnimation = Animation(3, numFrame, animationSpeed);

            animations.emplace("DownAnimation", downAnimation);
            animations.emplace("RightAnimation", rightAnimation);
            animations.emplace("LeftAnimation", leftAnimation);
            animations.emplace("UpAnimation", upAnimation);

            this->animationIndex = 0;
            this->currentAnimationName = "DownAnimation";
        }else{
            Animation singleAnimation = Animation(0, numFrame, animationSpeed);
            animations.emplace("SingleAnimation", singleAnimation);
            this->animationIndex = 0;
            this->currentAnimationName = "SingleAnimation";
        }
        Play(this->currentAnimationName);
        SetTexture(id);
    }

    void Play(std::string animationName) {
        numFrame = animations[animationName].numFrame;
        animationIndex = animations[animationName].index;
        animationSpeed = animations[animationName].animationSpeed;
        currentAnimationName = animationName;
    }

    void SetTexture(std::string assetTextureId) {
        texture = Game::assetManager->GetTexture(assetTextureId);
    }

    void Initialize() override {
        transform = owner->GetComponent<TransformComponent>();
        sourceRectangle.x = 0;
        sourceRectangle.y = 0;
        sourceRectangle.w = transform->width;
        sourceRectangle.h = transform->height;
    }

    void Update(float deltaTime) override {
        if(isAnimated) {
            sourceRectangle.x = (sourceRectangle.w * static_cast<int>((SDL_GetTicks() / animationSpeed) % numFrame));
        }
        sourceRectangle.y = animationIndex * transform->height;
        destinationRectangle.x = static_cast<int>(transform->position.x);
        destinationRectangle.y = static_cast<int>(transform->position.y);
        destinationRectangle.w = transform->width * transform->scale;
        destinationRectangle.h = transform->height * transform->scale;
    }

    void Render() override {
        TextureManager::Draw(texture, sourceRectangle, destinationRectangle, spriteFlip);
    }
};

#endif