#include "./Animation.h"

Animation::Animation(){}

Animation::Animation(int index, int numFrame, int animationSpeed){
    this->index = index;
    this->numFrame = numFrame;
    this->animationSpeed = animationSpeed;
}