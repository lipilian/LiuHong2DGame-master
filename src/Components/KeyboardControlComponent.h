#ifndef KEYBOARDCONTROLCOMPONENT_H
#define KEYBOARDCONTROLCOMPONENT_H

#include "../Game.h"
#include "../EntityManager.h"
#include "./TransformComponent.h"
#include "./SpriteComponent.h"
#include "./ColliderComponent.h"
#include "../Constants.h"

class KeyboardControlComponent : public Component {
    public:
        std::string upKey;
        std::string downKey;
        std::string leftKey;
        std::string rightKey;
        std::string shootKey;
        std::string collisionKey;
        TransformComponent *transform;
        SpriteComponent *sprite;
        ColliderComponent *collider;

        KeyboardControlComponent(){}

        KeyboardControlComponent(std::string collisionKey){
            this->collisionKey = GetSDLKeyStringCode(collisionKey);
        }

        KeyboardControlComponent(std::string upKey, std::string rightKey, std::string downKey, 
                    std::string leftKey, std::string shootKey, std::string collisionKey){
            this->upKey = GetSDLKeyStringCode(upKey);
            this->rightKey = GetSDLKeyStringCode(rightKey);
            this->downKey = GetSDLKeyStringCode(downKey);
            this->leftKey = GetSDLKeyStringCode(leftKey);
            this->shootKey = GetSDLKeyStringCode(shootKey);
            this->collisionKey = GetSDLKeyStringCode(collisionKey);
        }

        std::string GetSDLKeyStringCode(std::string key){
            if (key.compare("up") == 0) return "1073741906";
            if (key.compare("down") == 0) return "1073741905";
            if (key.compare("left") == 0) return "1073741904";
            if (key.compare("right") == 0) return "1073741903";
            if (key.compare("space") == 0) return "32"; 
            if (key.compare("collisionKey") == 0) return "99";
            return std::to_string(static_cast<int>(key[0]));
        }

        void Initialize() override {
            transform = owner->GetComponent<TransformComponent>();
            sprite = owner->GetComponent<SpriteComponent>();
            collider = owner->GetComponent<ColliderComponent>();
        }

        void Update(float deltaTime) override {
            if(Game::event.type == SDL_KEYDOWN) {
                std::string keyCode = std::to_string(Game::event.key.keysym.sym);

                if(keyCode.compare(upKey) == 0) {
                    transform->velocity.y = -30;
                    transform->velocity.x = 0;
                    sprite->Play("UpAnimation");
                }
                if(keyCode.compare(downKey) == 0) {
                    transform->velocity.y = 30;
                    transform->velocity.x = 0;
                    sprite->Play("DownAnimation");
                }
                if(keyCode.compare(leftKey) == 0){
                    transform->velocity.y = 0;
                    transform->velocity.x = -30;
                    sprite->Play("LeftAnimation");
                }
                if(keyCode.compare(rightKey) == 0){
                    transform->velocity.y = 0;
                    transform->velocity.x = 30;
                    sprite->Play("RightAnimation");
                }
                if(keyCode.compare(collisionKey) == 0){
                    collider->keyPressed = true;
                }
                if(keyCode.compare(shootKey) == 0){
                    //TODU: shoot missing
                }
            }
            
            if(Game::event.type == SDL_KEYUP) {
                std::string keyCode = std::to_string(Game::event.key.keysym.sym);

                if(keyCode.compare(upKey) == 0) {
                    transform->velocity.y = 0;
                }
                if(keyCode.compare(downKey) == 0) {
                    transform->velocity.y = 0;
                }
                if(keyCode.compare(leftKey) == 0){
                    transform->velocity.x = 0;
                }
                if(keyCode.compare(rightKey) == 0){
                    transform->velocity.x = 0;
                }
                if(keyCode.compare(collisionKey) == 0){
                    collider->keyPressed = false;
                }
            }
        }
};

#endif