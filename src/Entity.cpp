#include "./Entity.h"
#include <iostream>

Entity::Entity(EntityManager& manager): manager(manager){
    this->isActive = true;
}

Entity::Entity(EntityManager& manager, std::string name, LayerType layer): manager(manager), name(name), layer(layer){
    this->isActive = true;
}

void Entity::Update(float deltaTime){
    for(auto& component: components){
        component->Update(deltaTime);
    }
}

void Entity::Render() {
    for(auto& component: components){
        component->Render();
    }
}

void Entity::ListAllComponents() const {
    for(auto& mapElement: componentTypeMap){
        std::cout << "Component<" << mapElement.first->name() << ">" << std::endl;
    }
}

void Entity::Destroy(){
    this->isActive = false;
}

bool Entity::IsActive() const {
    return this->isActive;
}