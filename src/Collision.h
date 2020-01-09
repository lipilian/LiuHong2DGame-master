#ifndef COLLISION_H
#define COLLISION_H

#include <SDL2/SDL.h>

class Collision {
    public:
        static bool CheckRectangleCollision(const SDL_Rect& RectA, const SDL_Rect& RectB);
};

#endif