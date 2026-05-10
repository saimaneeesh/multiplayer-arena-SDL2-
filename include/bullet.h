#ifndef BULLET_H
#define BULLET_H

#include "transform.h"

struct Bullet
{
    Transform transform;
    int size;
    float velocityX;
    float velocityY;
    int ownerId;
};

#endif