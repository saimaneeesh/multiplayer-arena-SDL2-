#ifndef STATE_H
#define STATE_H

#include "player.h"
#include "block.h"
#include "bullet.h"
#include <vector>

struct GameState
{
    std::vector<Player> players;
    std::vector<Block> blocks;
    std::vector<Bullet> bullets;
};

#endif