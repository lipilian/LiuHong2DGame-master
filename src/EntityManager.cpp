#include "./EntityManager.h"
#include "./Collision.h"
#include "./Components/ColliderComponent.h"

void EntityManager::ClearData() {
    for(auto& entity : entities) {
        entity->Destroy();
    }
}

bool EntityManager::HasNoEntities() {
    return entities.empty();
}

void EntityManager::Update(float deltaTime) {
    for (int i = 0; i < entities.size(); i++) {
        if(!entities[i]->IsActive()){
            entities.erase(entities.begin() + i);
        } else {
            entities[i]->Update(deltaTime);
        }
    }
}

void EntityManager::Render() { 
    for (int layerNum = 0; layerNum < NUM_LAYERS; ++layerNum) {
        for (auto& entity : GetEntitiesBylayer(static_cast<LayerType>(layerNum))) {
            entity->Render();
        }
    }
}

Entity& EntityManager::AddEntity(std::string entityName, LayerType layer) {
    Entity *entity = new Entity(*this, entityName, layer);
    entities.emplace_back(entity);
    return *entity;
}


std::vector<Entity*> EntityManager::GetEntities() const { 
    return entities;
}

std::vector<Entity*> EntityManager::GetEntitiesBylayer(LayerType layer) const {
    std::vector<Entity*> selectedEntities;
    for(auto& entity : entities) {
        if (entity->layer == layer){
            selectedEntities.emplace_back(entity);
        }
    }
    return selectedEntities;
}

void EntityManager::ListAllEntities() const {
    unsigned int i = 0;
    for(auto& entity : entities){
        //std::cout<< "Entity[" << i << "]:" << entity->name << std::endl;
        //entity->ListAllComponents();
        i++;
    }
}

CollisionType EntityManager::CheckCollisions() const {
    for(auto& thisEntity: entities) {
        if(thisEntity->HasComponent<ColliderComponent>()){
            ColliderComponent* thisCollider = thisEntity->GetComponent<ColliderComponent>();
            for(auto& thatEntity: entities){
                if(thisEntity->name.compare(thatEntity->name) != 0 && thatEntity->HasComponent<ColliderComponent>()){
                    ColliderComponent* thatCollider = thatEntity->GetComponent<ColliderComponent>();
                    if(Collision::CheckRectangleCollision(thisCollider->collider,thatCollider->collider)){
                        if(thisCollider->colliderTag.compare("player") == 0 && 
                        thatCollider->colliderTag.compare("enemy") == 0){
                            return PLAYER_ENEMY_COLLISION;
                        }
                        if(thisCollider->colliderTag.compare("player") == 0 &&
                        thatCollider->colliderTag.compare("projectile") == 0){
                            return PLAYER_PROJECTILE_COLLISION;
                        }
                        if(thisCollider->colliderTag.compare("enemy") == 0 &&
                        thatCollider->colliderTag.compare("friendly_projectile") == 0){
                            return ENEMY_PROJECTILE_COLLISION;
                        }
                        if(thisCollider->colliderTag.compare("player") == 0 && 
                        thatCollider->colliderTag.compare("level_complete") == 0){
                            return PLAYER_LEVEL_COMPLETE_COLLISION;
                        }
                    }
                }
            }
        }
    }
    return NO_COLLISION;
}

/*
std::string EntityManager::CheckEntityCollisions(Entity& myEntity) const{
    ColliderComponent* myCollider = myEntity.GetComponent<ColliderComponent>();
    for(auto& entity: entities) {
        if(entity->name.compare(myEntity.name) != 0 && entity->name.compare("Tile") != 0){
            if(entity->HasComponent<ColliderComponent>()){
                ColliderComponent* otherCollider = entity->GetComponent<ColliderComponent>();
                if(Collision::CheckRectangleCollision(myCollider->collider, otherCollider->collider)){
                    return otherCollider->colliderTag;
                }
            }
        }
    }
    return std::string();
}
*/

unsigned int EntityManager::GetEntityCount() {
    return entities.size();
}