#ifndef ANIMATION_H
#define ANIMATION_H

class Animation {
    public:
    unsigned int index;
    unsigned int numFrame;
    unsigned int animationSpeed;
    Animation();
    Animation(int index, int numFrame, int animationSpeed);

};

#endif