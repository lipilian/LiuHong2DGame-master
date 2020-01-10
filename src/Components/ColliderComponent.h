#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include "../Game.h"
#include "../EntityManager.h"
#include "./TransformComponent.h"
#include "../TextureManager.h"
#include "../AssetManager.h"
#include <SDL2/SDL.h>

class ColliderComponent : public Component {
    private:
        SDL_Texture* texture;   
    public:
        std::string colliderTag;
        SDL_Rect collider;
        SDL_Rect sourceRectangle;
        SDL_Rect destinationRectangle;
        TransformComponent* transform;
        bool keyPressed;

        ColliderComponent(std::string colliderTag, int x, int y, int width, int height, std::string id, bool keyPressed){
            this->colliderTag = colliderTag;
            this->collider = {x,y,width,height};
            this->keyPressed = keyPressed;
            texture = Game::assetManager->GetTexture(id);
        }

        void Initialize() override {
            if(owner->HasComponent<TransformComponent>()){
                transform = owner->GetComponent<TransformComponent>();
                sourceRectangle = {0,0,transform->width,transform->height};
                destinationRectangle = {collider.x,collider.y,collider.w,collider.h};
            }
        }

        void Update(float deltaTime) override {
            collider.x = static_cast<int>(transform->position.x);
            collider.y = static_cast<int>(transform->position.y);
            collider.w = transform->width * transform->scale;
            collider.h = transform->height * transform->scale;
            destinationRectangle.x = collider.x - Game::camera.x;
            destinationRectangle.y = collider.y - Game::camera.y;
        }

        
        void Render() override {
            if(keyPressed){
                TextureManager::Draw(texture, sourceRectangle, destinationRectangle, SDL_FLIP_NONE);
            }
        }
};
#endif