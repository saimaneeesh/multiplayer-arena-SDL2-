#ifndef PLAYER_H
#define PLAYER_H

#include "transform.h"

struct Player
{
    Transform transform;
    int width;
    int height;
    float velocityX;
    float velocityY;
    int health = 100;
    int score = 0;
    bool isGrounded = false;
    float shootTimer = 0.0f;
};

#endif