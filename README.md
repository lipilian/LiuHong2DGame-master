# LiuHong2DGame-master
# Introduction
This project use SDL library and Lua embeded C++ code create 2D game Engine about 2D game. The project is built based on Mac
OS system
# Denpendency
Install sdl2:
```
$brew install sdl2
```
install sdl2 image:
```
$brew install sdl2_image
```
Install sdl2 font support:
```
$brew install sdl2_ttf
```
Install sdl2 mixer:
```
$brew install sdl2_mixer
```
install lua:
```
$brew install lua
```
# Game Loop (main.cpp and Game.cpp)
1. Game initialize: SDL, TTF, window, SDL renderer, Load all component;
2. Fix time step for multicomputer(Game::Update());
3. Double render buffer(Game::Render());

# Entity and their Component
1. Create Entity manager to load entity into entity vector.
2. Create Asset manager to manager the assets from the file.
3. Multi component class inherit from component main class :
* TransformComponent.h : control the transform of entity
* TileComponent.h : tileMap draw based on camera position
* TextLabelComponent.h : font in the game
* SpriteComponent.h : animation of entity
* ProjectileEmitterComponent : shooting of enemy and player
* KeyboardControlComponent.h: Process input from keyboard
* ColliderComponent.h : collision with other entities.

# Render order define with Layer_type

# Embed Lua
Lua Load the Entity from Lua level file

# Example video
You can check the demo video for single enemy game engine [Here !](https://www.youtube.com/watch?v=YNJEQxZranw)
