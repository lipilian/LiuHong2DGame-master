//
// Created by hong liu on 12/31/19.
//

#include <iostream>
#include "./Constants.h"
#include "./Game.h"
#include "./Components/TransformComponent.h"
#include "../lib/glm/glm.hpp"
#include "./AssetManager.h"
#include "./Map.h"
#include "./Components/SpriteComponent.h"
#include "./Components/KeyboardControlComponent.h"
#include "./Components/ColliderComponent.h"
#include "./Components/TextLabelComponent.h"
#include "./Components/ProjectileEmitterComponent.h"

EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Entity* mainPlayer = NULL;
Map* map;


Game::Game(){
    this->isRunning = false;
}

Game::~Game(){
}

bool Game::IsRunning() const {
    return this->isRunning;
}

void Game::Initialize(int width, int height){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error Initializing SDL.." << std::endl;
        return;
    }
    if (TTF_Init() != 0) {
        std::cerr << "Error Initializing TTF.." << std::endl;
        return;
    }
    window = SDL_CreateWindow(
            NULL,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            SDL_WINDOW_BORDERLESS
            );
    if(!window){
        std::cerr << "Error creating SDL window.." << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer){
        std::cerr << "Error creating SDL renderer.." << std::endl;
        return;
    }

    LoadLevel(1);
    isRunning = true;
    return;
}

//Entity& player(manager.AddEntity("chopper", PLAYER_LAYER));

void Game::LoadLevel(int levelNumber) {
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);

    std::string levelName = "Level" + std::to_string(levelNumber);
    lua.script_file("./assets/scripts/" + levelName + ".lua");

    /*********************************************/
    /* LOADS ASSETS FROM LUA CONFIG FILE         */
    /*********************************************/
    sol::table levelData = lua[levelName];
    sol::table levelAssets = levelData["assets"];   
 
    unsigned int assetIndex = 0;
    while (true) {
        sol::optional<sol::table> existsAssetIndexNode = levelAssets[assetIndex]; 
        if (existsAssetIndexNode == sol::nullopt) {
            break;
        } else {
            sol::table asset = levelAssets[assetIndex];
            std::string assetType = asset["type"];
            if (assetType.compare("texture") == 0) {
                std::string assetId = asset["id"];
                std::string assetFile = asset["file"];
                assetManager->AddTexture(assetId, assetFile.c_str());
            } 
        }
        assetIndex++;
    }

    /*********************************************/
    /* LOADS MAP FROM LUA CONFIG FILE            */
    /*********************************************/
    sol::table levelMap = levelData["map"];
    std::string mapTextureId = levelMap["textureAssetId"];
    std::string mapFile = levelMap["file"];

    map = new Map(
        mapTextureId,
        static_cast<int>(levelMap["scale"]),
        static_cast<int>(levelMap["tileSize"])
    );

    map->LoadMap(
        mapFile,
        static_cast<int>(levelMap["mapSizeX"]),
        static_cast<int>(levelMap["mapSizeY"])
    );

    /*********************************************/
    /* LOADS Entites and their components           */
    /*********************************************/
    sol::table levelEntities = levelData["entities"];
    unsigned int entityIndex = 0;
    while(true){
        sol::optional<sol::table> existsEntityIndexNode = levelEntities[entityIndex];
        if (existsEntityIndexNode == sol::nullopt) {
            break;
        } else {
            sol::table entity = levelEntities[entityIndex];
            std::string entityName = entity["name"];
            LayerType entityLayerType = static_cast<LayerType>(static_cast<int>(entity["layer"]));
            //Add new entity
            auto &newEntity(manager.AddEntity(entityName, entityLayerType));
            //Add transform component
            sol::optional<sol::table> existsTransformComponent = entity["components"]["transform"];
            if(existsTransformComponent != sol::nullopt) {
                newEntity.AddComponent<TransformComponent>(
                    static_cast<int>(entity["components"]["transform"]["position"]["x"]),
                    static_cast<int>(entity["components"]["transform"]["position"]["y"]),
                    static_cast<int>(entity["components"]["transform"]["velocity"]["x"]),
                    static_cast<int>(entity["components"]["transform"]["velocity"]["y"]),
                    static_cast<int>(entity["components"]["transform"]["width"]),
                    static_cast<int>(entity["components"]["transform"]["height"]),
                    static_cast<int>(entity["components"]["transform"]["scale"])
                );
            }
            //Add sprite component
            sol::optional<sol::table> existsSpriteComponent = entity["components"]["sprite"];
            if (existsSpriteComponent != sol::nullopt) {
                std::string textureId = entity["components"]["sprite"]["textureAssetId"];
                bool isAnimated = entity["components"]["sprite"]["animated"];
                if (isAnimated) {
                    newEntity.AddComponent<SpriteComponent>(
                        textureId,
                        static_cast<int>(entity["components"]["sprite"]["frameCount"]),
                        static_cast<int>(entity["components"]["sprite"]["animationSpeed"]),
                        static_cast<bool>(entity["components"]["sprite"]["hasDirections"]),
                        static_cast<bool>(entity["components"]["sprite"]["fixed"])
                    );
                } else {
                    newEntity.AddComponent<SpriteComponent>(textureId, false);
                }
            }
            //Add collider components
            sol::optional<sol::table> existsColliderComponent = entity["components"]["collider"];
            if (existsColliderComponent != sol::nullopt) {
                std::string colliderTag = entity["components"]["collider"]["tag"];
                bool boundingBox = entity["components"]["collider"]["boundingBox"];
                if(boundingBox){
                    newEntity.AddComponent<ColliderComponent>(
                        colliderTag,
                        static_cast<int>(entity["components"]["transform"]["position"]["x"]),
                        static_cast<int>(entity["components"]["transform"]["position"]["y"]),
                        static_cast<int>(entity["components"]["transform"]["width"]),
                        static_cast<int>(entity["components"]["transform"]["height"]),
                        "collision-boundingBox",
                        false
                    );
                } else {
                    newEntity.AddComponent<ColliderComponent>(
                        colliderTag,
                        static_cast<int>(entity["components"]["transform"]["position"]["x"]),
                        static_cast<int>(entity["components"]["transform"]["position"]["y"]),
                        static_cast<int>(entity["components"]["transform"]["width"]),
                        static_cast<int>(entity["components"]["transform"]["height"])
                    );
                }
            } 
            //Add Control componets
            sol::optional<sol::table> existsInputComponent = entity["components"]["input"];
            if (existsInputComponent != sol::nullopt) {
                sol::optional<sol::table> existsKeyboardInputComponent = entity["components"]["input"]["keyboard"];
                if (existsKeyboardInputComponent != sol::nullopt) {
                    if(entityName.compare("player") == 0){
                        std::string upKey = entity["components"]["input"]["keyboard"]["up"];
                        std::string rightKey = entity["components"]["input"]["keyboard"]["right"];
                        std::string downKey = entity["components"]["input"]["keyboard"]["down"];
                        std::string leftKey = entity["components"]["input"]["keyboard"]["left"];
                        std::string shootKey = entity["components"]["input"]["keyboard"]["shoot"];
                        std::string collisionKey = entity["components"]["input"]["keyboard"]["collision"];
                        newEntity.AddComponent<KeyboardControlComponent>(upKey, rightKey, downKey, leftKey, shootKey, collisionKey);
                    } else {
                        std::string collisionKey = entity["components"]["input"]["keyboard"]["collision"];
                        newEntity.AddComponent<KeyboardControlComponent>(collisionKey);
                    }
                }
            }
        }
        entityIndex++;
    }
    mainPlayer = manager.GetEntityByName("player");
}
    /* //Start including new assets to the assetmanager list 
    assetManager->AddTexture("tank-image", std::string("./assets/images/tank-big-right.png").c_str());
    assetManager->AddTexture("chopper-image", std::string("./assets/images/chopper-spritesheet.png").c_str());
    assetManager->AddTexture("radar-image", std::string("./assets/images/radar.png").c_str());
    assetManager->AddTexture("jungle-tileTexture", std::string("./assets/tilemaps/jungle.png").c_str());
    assetManager->AddTexture("collision-boundingBox", std::string("./assets/images/collision-texture.png").c_str());
    assetManager->AddTexture("heliport-image", std::string("./assets/images/heliport.png").c_str());
    assetManager->AddTexture("projectile-image", std::string("./assets/images/bullet-enemy.png").c_str());
    assetManager->AddFont("charriot-font", std::string("./assets/fonts/charriot.ttf").c_str(), 14);

    map = new Map("jungle-tileTexture", 2, 32);
    map->LoadMap("./assets/tilemaps/jungle.map", 25, 20);

    //Start including entities and also components to them 
    player.AddComponent<TransformComponent>(240, 106, 0, 0, 32, 32, 1);
    player.AddComponent<SpriteComponent>("chopper-image", 2, 90, true, false);
    player.AddComponent<ColliderComponent>("player", 240, 106, 32, 32, "collision-boundingBox", false);
    player.AddComponent<KeyboardControlComponent>("up", "right", "down", "left", "space", "collisionKey");

    Entity& heliport(manager.AddEntity("heliport", OBSTACLE_LAYER));
    heliport.AddComponent<TransformComponent>(470, 420, 0, 0, 32, 32, 1);
    heliport.AddComponent<SpriteComponent>("heliport-image");
    heliport.AddComponent<ColliderComponent>("level_complete", 470, 420, 32, 32, "collision-boundingBox", false);
    heliport.AddComponent<KeyboardControlComponent>("collisionKey");

    Entity& tankEntity(manager.AddEntity("tank", ENEMY_LAYER));
    tankEntity.AddComponent<TransformComponent>(150, 495, 10, 0, 32, 32, 1);
    tankEntity.AddComponent<SpriteComponent>("tank-image");
    tankEntity.AddComponent<ColliderComponent>("enemy", 150, 495, 32, 32, "collision-boundingBox", false);
    tankEntity.AddComponent<KeyboardControlComponent>("collisionKey");

    Entity& projectile(manager.AddEntity("projectile", PROJECTILE_LAYER));
    projectile.AddComponent<TransformComponent>(150 + 16, 495 + 16, 0, 0, 4, 4, 1);
    projectile.AddComponent<SpriteComponent>("projectile-image");
    projectile.AddComponent<ColliderComponent>("projectile", 150 + 16, 495 + 16, 4, 4, "collision-boundingBox", false);
    projectile.AddComponent<ProjectileEmitterComponent>(50, 270, 200, true);

    Entity& radarEntity(manager.AddEntity("Radar", UI_LAYER));
    radarEntity.AddComponent<TransformComponent>(720, 15, 0, 0, 64, 64, 1);
    radarEntity.AddComponent<SpriteComponent>("radar-image", 8, 150, false, true);

    Entity& labelLevelName(manager.AddEntity("LabelLevelName", UI_LAYER));
    labelLevelName.AddComponent<TextLabelComponent>(10, 10, "First Level...", "charriot-font", WHITE_COLOR);
}
*/
void Game::ProcessInput() {
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT: {
            isRunning = false;
            break;
        }
        case SDL_KEYDOWN: {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                isRunning = false;
            }
        }
        default: {
            break;
        }
    }
}

void Game::Update(){
    // Wait until 16ms has ellapsed since the last frame
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TARGET_TIME));
    //simple while loop to match the target frame rate
    
    // Delta time is the difference between frames, unit is second
    float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

    // Clamp deltaTime to a maximum value
    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

    // Update new ticks for next pass
    ticksLastFrame = SDL_GetTicks(); // return the ms of current time

    manager.Update(deltaTime);

    // Handle camera movement
    HandleCameraMovement();
    CheckCollisions();
}

void Game::Render(){
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    if(manager.HasNoEntities()){
        return;
    } 
    manager.Render();
    
    SDL_RenderPresent(renderer);
}

void Game::HandleCameraMovement(){
    if (mainPlayer){
        TransformComponent* mainPlayerTransform = mainPlayer->GetComponent<TransformComponent>();
        camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH / 2);
        camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

        //clamping the value of camera
        camera.x = camera.x < 0 ? 0 : camera.x;
        camera.y = camera.y < 0 ? 0 : camera.y;
        camera.x = camera.x > camera.w ? camera.w : camera.x;
        camera.y = camera.y > camera.h ? camera.h : camera.y;
    }
}

void Game::CheckCollisions(){
    CollisionType collisionType = manager.CheckCollisions();
    if (collisionType == PLAYER_ENEMY_COLLISION){
        ProcessGameOver();
    }
    if (collisionType == PLAYER_PROJECTILE_COLLISION){
        ProcessGameOver();
    }
    if (collisionType == PLAYER_LEVEL_COMPLETE_COLLISION){
        ProcessNextLevel(1);
    }
}

void Game::ProcessGameOver(){
    std::cout << "Game Over" << std::endl;
    isRunning = false;
}

void Game::ProcessNextLevel(int levelNumber){
    std::cout << "Next Level" << std::endl;
    isRunning = false;
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
