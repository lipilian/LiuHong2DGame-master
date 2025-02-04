#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "./Entity.h"
#include "./Component.h"
#include <vector>
#include <iostream>
#include "./Constants.h"

class EntityManager {
    private:
    std::vector<Entity*> entities;
    
    public:
    void ClearData();
    void Update(float deltaTime);
    void Render();
    bool HasNoEntities();
    Entity& AddEntity(std::string entityName, LayerType layer);
    std::vector<Entity*> GetEntities() const;
    std::vector<Entity*> GetEntitiesBylayer(LayerType layer) const;
    Entity* GetEntityByName(std::string entityName) const;
    void ListAllEntities() const;
    unsigned int GetEntityCount();
    CollisionType CheckCollisions() const;
};

#endif