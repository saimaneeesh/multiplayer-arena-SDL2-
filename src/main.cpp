#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "player.h"
#include "block.h"
#include "transform.h"
#include "bullet.h"
#include "input.h"
#include "state.h"


bool isOverlapping(const Player& a, const Block& b)
        {
            float aLeft   = a.transform.x - a.width / 2.0f;
            float aRight  = a.transform.x + a.width / 2.0f;
            float aTop    = a.transform.y - a.height / 2.0f;
            float aBottom = a.transform.y + a.height / 2.0f;

            float bLeft   = b.transform.x - b.width / 2.0f;
            float bRight  = b.transform.x + b.width / 2.0f;
            float bTop    = b.transform.y - b.height / 2.0f;
            float bBottom = b.transform.y + b.height / 2.0f;

            return (aRight > bLeft &&
                    aLeft  < bRight &&
                    aBottom > bTop &&
                    aTop    < bBottom);
        }

bool isOverlappingPlayer(const Player& a, const Player& b)
{
    float aLeft   = a.transform.x - a.width / 2.0f;
    float aRight  = a.transform.x + a.width / 2.0f;
    float aTop    = a.transform.y - a.height / 2.0f;
    float aBottom = a.transform.y + a.height / 2.0f;

    float bLeft   = b.transform.x - b.width / 2.0f;
    float bRight  = b.transform.x + b.width / 2.0f;
    float bTop    = b.transform.y - b.height / 2.0f;
    float bBottom = b.transform.y + b.height / 2.0f;

    return (aRight > bLeft &&
            aLeft  < bRight &&
            aBottom > bTop &&
            aTop    < bBottom);
}
 
int main()
{
    GameState state;
    
    Block block1;
    Block block2;
    Block block3;
    Block block4;

    state.players.emplace_back();
    state.players.emplace_back();
    state.players[0].health = 100;
    state.players[0].transform.x = 400;
    state.players[0].transform.y = 300;
    state.players[0].width = 10;
    state.players[0].height = 10;
    state.players[0].velocityX = 0;
    state.players[0].velocityY = 0;

    state.players[1].health = 100;
    state.players[1].transform.x = 600;
    state.players[1].transform.y = 200;
    state.players[1].width = 10;
    state.players[1].height = 10;
    state.players[1].velocityX = 0;
    state.players[1].velocityY = 0;
    
    state.blocks.emplace_back();
    state.blocks[0].transform.x = 400;
    state.blocks[0].transform.y = 200;
    state.blocks[0].width = 150;
    state.blocks[0].height = 20;

    state.blocks.emplace_back();
    state.blocks[1].transform.x = 600;
    state.blocks[1].transform.y = 300;
    state.blocks[1].width = 150;
    state.blocks[1].height = 20;

    state.blocks.emplace_back();
    state.blocks[2].transform.x = 500;
    state.blocks[2].transform.y = 400;
    state.blocks[2].width = 100;
    state.blocks[2].height = 20;

    state.blocks.emplace_back();
    state.blocks[3].transform.x = 200;
    state.blocks[3].transform.y = 500;
    state.blocks[3].width = 150;
    state.blocks[3].height = 20;


    std::vector<Input> inputs(state.players.size());

    float gravity = 1500.0f;
    float thrust = 3500.0f;
    float drag = 3.0f;
        
    SDL_Rect arena;

    arena.x = 50;
    arena.y = 50;
    arena.w = 700;
    arena.h = 500;


    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow(
        "My Game Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        0
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    bool running = true;

    SDL_Event event;

    Uint32 lastTime = SDL_GetTicks();

    float shootCooldown = 0.45f;
    bool zerogravity = false;

    SDL_Texture* background = IMG_LoadTexture(renderer, "assets/images/sky.jpg");
    SDL_Texture* spaceBg   = IMG_LoadTexture(renderer, "assets/images/space.jpg");
        
    while(running)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                running = false;
        }
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime; 
        float damping = 3.0f;

        const Uint8* keystate = SDL_GetKeyboardState(NULL);

        for(auto& player : state.players)
        {
            player.isGrounded = false;
        }
        for(auto& player : state.players)
        {
            player.shootTimer -= deltaTime;
        }
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
    
       

        float bulletSpeed = 1000.0f;

        inputs[0].up    = keystate[SDL_SCANCODE_W];
        inputs[0].down  = keystate[SDL_SCANCODE_S];
        inputs[0].left  = keystate[SDL_SCANCODE_A];
        inputs[0].right = keystate[SDL_SCANCODE_D];
        inputs[0].shoot = keystate[SDL_SCANCODE_SPACE];

        inputs[1].up    = keystate[SDL_SCANCODE_UP];
        inputs[1].down  = keystate[SDL_SCANCODE_DOWN];
        inputs[1].left  = keystate[SDL_SCANCODE_LEFT];
        inputs[1].right = keystate[SDL_SCANCODE_RIGHT];
        inputs[1].shoot = keystate[SDL_SCANCODE_R];

        for(int i = 0; i < state.players.size(); i++)
        {
            if(inputs[i].shoot && state.players[i].shootTimer <= 0.0f)
            {
                Bullet b;
                float dirX = mouseX - state.players[i].transform.x;
                float dirY = mouseY - state.players[i].transform.y;

                float length = sqrt(dirX * dirX + dirY * dirY);

                b.size = 4;

                if(length != 0){
                    dirX /= length;
                    dirY /= length;
                }

                b.transform.x = state.players[i].transform.x;
                b.transform.y = state.players[i].transform.y; 

                b.velocityX = dirX * bulletSpeed;
                b.velocityY = dirY * bulletSpeed;
                b.ownerId = i;
                
                state.bullets.push_back(b);
                state.players[i].shootTimer = shootCooldown;   
            }
        }
    
        if (keystate[SDL_SCANCODE_G])
        {
            zerogravity = true;
        }
        if (keystate[SDL_SCANCODE_N])
        {
            zerogravity = false;
        }

        for(int i = 0; i<state.players.size(); i++){

            if(!zerogravity){
                if(inputs[i].up){
                    state.players[i].velocityY -= thrust * deltaTime;
                }
                if(inputs[i].down){
                    state.players[i].velocityY += thrust * deltaTime;
                }
                if(inputs[i].left){
                    state.players[i].velocityX -= thrust * deltaTime;
                }   
                if(inputs[i].right){
                    state.players[i].velocityX += thrust * deltaTime;
                }
            }
        }
             
        for (int i = 0; i < state.bullets.size(); i++)
        {
            Bullet &b = state.bullets[i];
            b.transform.x += b.velocityX * deltaTime;
            b.transform.y += b.velocityY * deltaTime;
        
            bool bulletDead = false;

            for (const Block& block : state.blocks)
            {
                float bulletLeft   = b.transform.x - b.size/2;
                float bulletRight  = b.transform.x + b.size/2;
                float bulletTop    = b.transform.y - b.size/2;
                float bulletBottom = b.transform.y + b.size/2;

                float blockLeft   = block.transform.x - block.width/2;
                float blockRight  = block.transform.x + block.width/2;
                float blockTop    = block.transform.y - block.height/2;
                float blockBottom = block.transform.y + block.height/2;

                bool overlap =   
                    bulletRight  > blockLeft &&
                    bulletLeft   < blockRight &&
                    bulletBottom > blockTop &&
                    bulletTop    < blockBottom;

                if (overlap)
                {
                    bulletDead = true;
                    break;
                }
                
            }
            
            if (b.transform.x < arena.x ||
                b.transform.x > arena.x + arena.w ||
                b.transform.y < arena.y ||
                b.transform.y > arena.y + arena.h)
            {
                bulletDead = true;
            }
            for(int j = 0; j < state.players.size(); j++)
            {
                if(j == b.ownerId)
                    continue;

                Player& player = state.players[j];

                float enemyLeft   = player.transform.x - player.width/2;
                float enemyRight  = player.transform.x + player.width/2;
                float enemyTop    = player.transform.y - player.height/2;
                float enemyBottom = player.transform.y + player.height/2;

                float bulletLeft   = b.transform.x - b.size/2;
                float bulletRight  = b.transform.x + b.size/2;
                float bulletTop    = b.transform.y - b.size/2;
                float bulletBottom = b.transform.y + b.size/2;

                bool hit =
                    bulletRight  > enemyLeft &&
                    bulletLeft   < enemyRight &&
                    bulletBottom > enemyTop &&
                    bulletTop    < enemyBottom;

                if(hit)
                {
                    player.health -= 10;
                    bulletDead = true;
                }

                if(player.health <= 0)
                {
                    player.transform.x = 400;
                    player.transform.y = 300;
                    player.health = 100;

                    state.players[b.ownerId].score += 10;
                }
            }
            if (bulletDead)
                {
                    state.bullets.erase(state.bullets.begin() + i);
                    i--;
                }

        }
        for(auto& player : state.players)
        {
            if(!zerogravity)
                player.velocityY += gravity * deltaTime;
        }
                float maxSpeedX = 600.0f;
                float maxSpeedY = 800.0f;

        for(auto& player : state.players)
        {
            if(player.velocityX > maxSpeedX) player.velocityX = maxSpeedX;
            if(player.velocityX < -maxSpeedX) player.velocityX = -maxSpeedX;

            if(player.velocityY > maxSpeedY) player.velocityY = maxSpeedY;
            if(player.velocityY < -maxSpeedY) player.velocityY = -maxSpeedY;
        }
        
                for(auto& player : state.players)
        {
            // movement
            player.transform.x += player.velocityX * deltaTime;
            player.transform.y += player.velocityY * deltaTime;

            // damping
            player.velocityX -= player.velocityX * damping * deltaTime;
            player.velocityY -= player.velocityY * damping * deltaTime;

            // boundaries
            float leftBoundary   = arena.x + player.width / 2;
            float rightBoundary  = arena.x + arena.w - player.width / 2;
            float topBoundary    = arena.y + player.height / 2;
            float bottomBoundary = arena.y + arena.h - player.height / 2;

            if(player.transform.x < leftBoundary)
            {
                player.transform.x = leftBoundary;
                player.velocityX = 0;
            }

            if(player.transform.x > rightBoundary)
            {
                player.transform.x = rightBoundary;
                player.velocityX = 0;
            }

            if(player.transform.y < topBoundary)
            {
                player.transform.y = topBoundary;
                player.velocityY = 0;
            }

            if(player.transform.y > bottomBoundary)
            {
                player.transform.y = bottomBoundary;
                player.velocityY = 0;
                player.isGrounded = true;
            }
        }

        for(auto& player : state.players)
        {   
            for (const Block& block : state.blocks){
            
            if(isOverlapping(player, block)){
                float blockTop = block.transform.y - block.height / 2.0f;
                float blockBottom = block.transform.y + block.height / 2.0f;        
            

                if(player.velocityY >= 0)
                {
                    player.transform.y = blockTop - player.height / 2.0f;
                    player.velocityY = 0;
                    player.isGrounded = true;
                }
                else
                {
                    player.transform.y = blockBottom + player.height / 2.0f;
                    player.velocityY = 0;

                }
            }
            }
        }

        for (int i = 0; i < state.players.size(); i++)
        {
            for (int j = i + 1; j < state.players.size(); j++)
            {
              
                Player& a = state.players[i];
                Player& b = state.players[j];

                if (isOverlappingPlayer(a, b))
                {
                    // simple separation (you can improve later)
                    if (a.velocityY >= 0)
                    {
                        float bTop = b.transform.y - b.height / 2.0f;
                        a.transform.y = bTop - a.height / 2.0f;
                        a.velocityY = 0;
                    }
                }
            }
        }

        
       

    
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

       if(!zerogravity)
        {
            SDL_RenderCopy(renderer, background, NULL, &arena);
        }
        else
        {
            SDL_RenderCopy(renderer, spaceBg, NULL, NULL);
        }
            
        
        if(!background){
            std::cout << "Failed to load background texture: " << IMG_GetError() << std::endl;
        }

        
        SDL_Rect Rect;
        Rect.x = state.players[0].transform.x - state.players[0].width/2;
        Rect.y = state.players[0].transform.y - state.players[0].height/2;
        Rect.w = state.players[0].width;
        Rect.h = state.players[0].height;

        SDL_Rect enemyRect;
        enemyRect.x = state.players[1].transform.x - state.players[1].width/2;
        enemyRect.y = state.players[1].transform.y - state.players[1].height/2;
        enemyRect.w = state.players[1].width;
        enemyRect.h = state.players[1].height;

    SDL_SetRenderDrawColor(renderer, 255,255,0,255);


        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &enemyRect);
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderFillRect(renderer, &Rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_Rect outlineRect = { 50, 50, 700, 500  };
       
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);

        SDL_RenderDrawRect(renderer, &outlineRect);
        
    
        for (const Block& block : state.blocks)
        {
             SDL_Rect blockRect;
             blockRect.x = block.transform.x - block.width / 2;
             blockRect.y = block.transform.y - block.height / 2;    
             blockRect.w = block.width;
             blockRect.h = block.height;

             SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
             SDL_RenderFillRect(renderer, &blockRect);
        }
        for (const Bullet &b : state.bullets)
            {
                SDL_Rect bulletRect;

                bulletRect.x = b.transform.x - b.size / 2;
                bulletRect.y = b.transform.y - b.size / 2;
                bulletRect.w = b.size;
                bulletRect.h = b.size;

                SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
                SDL_RenderFillRect(renderer, &bulletRect);
            }
        state.bullets.erase(
            std::remove_if(state.bullets.begin(), state.bullets.end(),
                [&](Bullet &b)
                {
                    return b.transform.x < 0 ||
                        b.transform.x > 800 ||
                        b.transform.y < 0 ||
                        b.transform.y > 600;
                }),
            state.bullets.end());
       

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}