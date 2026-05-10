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
    std::vector<Player> players;
    std::vector<Block> blocks;
    std::vector<Bullet> bullets;
    
    Block block1;
    Block block2;
    Block block3;
    Block block4;

    players.emplace_back();
    players.emplace_back();
    players[0].health = 100;
    players[0].transform.x = 400;
    players[0].transform.y = 300;
    players[0].width = 10;
    players[0].height = 10;
    players[0].velocityX = 0;
    players[0].velocityY = 0;

    players[1].health = 100;
    players[1].transform.x = 600;
    players[1].transform.y = 200;
    players[1].width = 10;
    players[1].height = 10;
    players[1].velocityX = 0;
    players[1].velocityY = 0;
    
    block1.transform.x = 400;
    block1.transform.y = 200;
    block1.width = 150;
    block1.height = 20;

    block2.transform.x = 600;
    block2.transform.y = 300;
    block2.width = 150;
    block2.height = 20;

    block3.transform.x = 500;
    block3.transform.y = 400;
    block3.width = 100;
    block3.height = 20;

    block4.transform.x = 200;
    block4.transform.y = 500;
    block4.width = 150;
    block4.height = 20;

    blocks.push_back(block1);
    blocks.push_back(block2);
    blocks.push_back(block3);       
    blocks.push_back(block4);

    std::vector<Input> inputs(players.size());

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

        for(auto& player : players)
        {
            player.isGrounded = false;
        }
        for(auto& player : players)
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

        for(int i = 0; i < players.size(); i++)
        {
            if(inputs[i].shoot && players[i].shootTimer <= 0.0f)
            {
                Bullet b;
                float dirX = mouseX - players[i].transform.x;
                float dirY = mouseY - players[i].transform.y;

                float length = sqrt(dirX * dirX + dirY * dirY);

                b.size = 4;

                if(length != 0){
                    dirX /= length;
                    dirY /= length;
                }

                b.transform.x = players[i].transform.x;
                b.transform.y = players[i].transform.y; 

                b.velocityX = dirX * bulletSpeed;
                b.velocityY = dirY * bulletSpeed;
                b.ownerId = i;
                
                bullets.push_back(b);
                players[i].shootTimer = shootCooldown;   
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

        for(int i = 0; i<players.size(); i++){

            if(!zerogravity){
                if(inputs[i].up){
                    players[i].velocityY -= thrust * deltaTime;
                }
                if(inputs[i].down){
                    players[i].velocityY += thrust * deltaTime;
                }
                if(inputs[i].left){
                    players[i].velocityX -= thrust * deltaTime;
                }   
                if(inputs[i].right){
                    players[i].velocityX += thrust * deltaTime;
                }
            }
        }
             
        for (int i = 0; i < bullets.size(); i++)
        {
            Bullet &b = bullets[i];
            b.transform.x += b.velocityX * deltaTime;
            b.transform.y += b.velocityY * deltaTime;
        
            bool bulletDead = false;

            for (const Block& block : blocks)
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
            for(auto& player : players)
            {   
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
               players[b.ownerId].score += 10;
            }
            }
            if (bulletDead)
                {
                    bullets.erase(bullets.begin() + i);
                    i--;
                }

        }
for(auto& player : players)
{
    if(!zerogravity)
        player.velocityY += gravity * deltaTime;
}
        float maxSpeedX = 600.0f;
        float maxSpeedY = 800.0f;

for(auto& player : players)
{
    if(player.velocityX > maxSpeedX) player.velocityX = maxSpeedX;
    if(player.velocityX < -maxSpeedX) player.velocityX = -maxSpeedX;

    if(player.velocityY > maxSpeedY) player.velocityY = maxSpeedY;
    if(player.velocityY < -maxSpeedY) player.velocityY = -maxSpeedY;
}
  
        for(auto& player : players)
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

        for(auto& player : players)
        {   
            for (const Block& block : blocks){
            
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

        for (int i = 0; i < players.size(); i++)
        {
            for (int j = i + 1; j < players.size(); j++)
            {
              
                Player& a = players[i];
                Player& b = players[j];

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
        Rect.x = players[0].transform.x - players[0].width/2;
        Rect.y = players[0].transform.y - players[0].height/2;
        Rect.w = players[0].width;
        Rect.h = players[0].height;

        SDL_Rect enemyRect;
        enemyRect.x = players[1].transform.x - players[1].width/2;
        enemyRect.y = players[1].transform.y - players[1].height/2;
        enemyRect.w = players[1].width;
        enemyRect.h = players[1].height;

    SDL_SetRenderDrawColor(renderer, 255,255,0,255);


        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &enemyRect);
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderFillRect(renderer, &Rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_Rect outlineRect = { 50, 50, 700, 500  };
       
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);

        SDL_RenderDrawRect(renderer, &outlineRect);
        
    
        for (const Block& block : blocks)
        {
             SDL_Rect blockRect;
             blockRect.x = block.transform.x - block.width / 2;
             blockRect.y = block.transform.y - block.height / 2;    
             blockRect.w = block.width;
             blockRect.h = block.height;

             SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
             SDL_RenderFillRect(renderer, &blockRect);
        }
        for (const Bullet &b : bullets)
            {
                SDL_Rect bulletRect;

                bulletRect.x = b.transform.x - b.size / 2;
                bulletRect.y = b.transform.y - b.size / 2;
                bulletRect.w = b.size;
                bulletRect.h = b.size;

                SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
                SDL_RenderFillRect(renderer, &bulletRect);
            }
        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
                [&](Bullet &b)
                {
                    return b.transform.x < 0 ||
                        b.transform.x > 800 ||
                        b.transform.y < 0 ||
                        b.transform.y > 600;
                }),
            bullets.end());
       

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}