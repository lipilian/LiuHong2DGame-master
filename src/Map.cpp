#include "./Map.h"
#include "./Game.h"
#include "./EntityManager.h"
#include <fstream>
#include "./Components/TileComponent.h"

extern EntityManager manager;

Map::Map(std::string textureId, int scale, int tileSize){
    this->textureId = textureId;
    this->scale = scale;
    this->tileSize = tileSize;
}

void Map::LoadMap(std::string filePath, int mapSizeX, int mapSizeY){
    std::fstream mapFile;
    mapFile.open(filePath);

    for(int y = 0; y < mapSizeY; y++) {
        for(int x = 0; x < mapSizeX; x++){
            char ch;
            mapFile.get(ch);
            int sourceRectY = atoi(&ch) * tileSize;
            mapFile.get(ch);
            int sourceRectX = atoi(&ch) * tileSize;
            AddTile(sourceRectX, sourceRectY, (x * scale) * tileSize, (y * scale) * tileSize);
            mapFile.ignore();
        }
    }
    mapFile.close();
}

void Map::AddTile(int SourceRectX, int SourceRectY, int x, int y){
    Entity& newTile(manager.AddEntity("Tile", TILEMAP_LAYER));
    newTile.AddComponent<TileComponent>(SourceRectX, SourceRectY, x, y, tileSize, scale, textureId);
}