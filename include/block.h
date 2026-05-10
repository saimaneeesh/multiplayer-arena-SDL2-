#ifndef BLOCK_H
#define BLOCK_H

#include "transform.h"

struct Block
{
    Transform transform;
    int width;
    int height;
};

#endif