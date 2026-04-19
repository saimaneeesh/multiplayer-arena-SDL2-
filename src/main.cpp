#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

struct Transform
{
    float x, y;
    float rotation;
    float scaleX, scaleY;
};

struct Player
{
    Transform transform;
    int width;
    int height;
    float velocityX;
    float velocityY;
    int health = 100;
    int score = 0;;
};

struct Block
{
    Transform transform;
    int width;
    int height;
};

struct Bullet
{
    Transform transform;
    int size;
    float velocityX;
    float velocityY;
};
int main()
{
    Player player;
    std::vector<Block> blocks;
    std::vector<Bullet> bullets;
    Block block1;
    Block block2;
    Block block3;
    Block block4;
    
    player.health = 100;
    player.transform.x = 400;
    player.transform.y = 300;
    player.width = 10;
    player.height = 10;
    player.velocityX = 0;
    player.velocityY = 0;

    Player player1;
    player1.health = 100;
    player1.transform.x = 600;
    player1.transform.y = 200;
    player1.width = 10;
    player1.height = 10;
    player1.velocityX = 0;
    player1.velocityY = 0;
    
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

    float gravity = 1500.0f;
    bool isGrounded = false;
    float thrust = 3500.0f;
    float drag = 3.0f;
    
    
    SDL_Rect arena;

    arena.x = 50;
    arena.y = 50;
    arena.w = 700;
    arena.h = 500;
    float leftBoundary = arena.x + player.width/2;
    float rightBoundary = arena.x + arena.w - player.width/2;
    float topBoundary = arena.y + player.height/2;
    float bottomBoundary = arena.y + arena.h - player.height/2;

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
    float shootTimer = 0.0f;
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

        const Uint8* keystate = SDL_GetKeyboardState(NULL);

        
        isGrounded = false;
        shootTimer -= deltaTime;
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        float dirX = mouseX - player.transform.x;
        float dirY = mouseY - player.transform.y;
        float length = sqrt(dirX * dirX + dirY * dirY);
        if (length != 0)
        {               
        dirX /= length;
        dirY /= length;
        }

        float bulletSpeed = 1000.0f;

        if (keystate[SDL_SCANCODE_SPACE] && shootTimer <= 0.0f)
        {
            Bullet b;
                float spawnOffset = 10;
                float recoilforce = 5.0f;
                b.size = 4;  

                if(zerogravity)
                    recoilforce = 200;

                player.velocityX -= dirX * recoilforce;
                player.velocityY -= dirY * recoilforce  ;

                b.transform.x = player.transform.x + dirX * spawnOffset;
                b.transform.y = player.transform.y + dirY * spawnOffset; 

                b.velocityX = dirX * bulletSpeed;
                b.velocityY = dirY * bulletSpeed;
                bullets.push_back(b);
                shootTimer = shootCooldown;   
        }

        if (keystate[SDL_SCANCODE_R] && shootTimer <= 0.0f)
        {
            Bullet b1;
                float spawnOffset = 10;
                float recoilforce = 5.0f;
                b1.size = 4;  

                if(zerogravity)
                    recoilforce = 200;

                player1.velocityX -= dirX * recoilforce;
                player1.velocityY -= dirY * recoilforce ;

                b1.transform.x = player1.transform.x + dirX * spawnOffset;
                b1.transform.y = player1.transform.y + dirY * spawnOffset; 

                b1.velocityX = dirX * bulletSpeed;
                b1.velocityY = dirY * bulletSpeed;
                bullets.push_back(b1);
                shootTimer = shootCooldown;   
        }
        if (keystate[SDL_SCANCODE_G])
        {
            zerogravity = true;
        }
        if (keystate[SDL_SCANCODE_N])
        {
            zerogravity = false;
        }
        
        if (!zerogravity)
        {
            if (keystate[SDL_SCANCODE_W])
                player.velocityY -= thrust * deltaTime;

            if (keystate[SDL_SCANCODE_S])
                player.velocityY += thrust * deltaTime;

            if (keystate[SDL_SCANCODE_A])
                player.velocityX -= thrust * deltaTime;

            if (keystate[SDL_SCANCODE_D])
                player.velocityX += thrust * deltaTime;
            if (keystate[SDL_SCANCODE_UP])
                player1.velocityY -= thrust * deltaTime;

            if (keystate[SDL_SCANCODE_DOWN])
                player1.velocityY += thrust * deltaTime;

            if (keystate[SDL_SCANCODE_LEFT])
                player1.velocityX -= thrust * deltaTime;

            if (keystate[SDL_SCANCODE_RIGHT])
                player1.velocityX += thrust * deltaTime;
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

            if (b.transform.x < leftBoundary ||
                    b.transform.x > rightBoundary ||   
                    b.transform.y < topBoundary ||
                    b.transform.y > bottomBoundary)
                {
                    bulletDead = true;
                }
                
            float enemyLeft   = player1.transform.x - player1.width/2;
            float enemyRight  = player1.transform.x + player1.width/2;
            float enemyTop    = player1.transform.y - player1.height/2;
            float enemyBottom = player1.transform.y + player1.height/2;
            
            float eLeft   = player.transform.x - player.width/2;
            float eRight  = player.transform.x + player.width/2;
            float eTop    = player.transform.y - player.height/2;
            float eBottom = player.transform.y + player.height/2;

            float bulletLeft   = b.transform.x - b.size/2;
            float bulletRight  = b.transform.x + b.size/2;
            float bulletTop    = b.transform.y - b.size/2;
            float bulletBottom = b.transform.y + b.size/2;

            bool hit =
                bulletRight  > enemyLeft &&
                bulletLeft   < enemyRight &&
                bulletBottom > enemyTop &&
                bulletTop    < enemyBottom;     
            bool ehit = 
                bulletRight  > eLeft &&
                bulletLeft   < eRight &&
                bulletBottom > eTop &&
                bulletTop    < eBottom;   
            if(hit)
            {
                player1.health -= 10;
                bulletDead = true;
            }  
            if(ehit)
            {
                player.health -= 10;
                bulletDead = true;
            }
            if(player1.health <= 0)
            {
                player1.transform.x = 400;
                player1.transform.y = 300;
                player1.health = 100;
                player.score += 10;
            }
            if(player.health <= 0)
            {
                player.transform.x = 400;
                player.transform.y = 300;
                player.health = 100;
                player1.score += 10;
            }
            if (bulletDead)
                {
                    bullets.erase(bullets.begin() + i);
                    i--;
                }

        }

        if (!zerogravity)
        {
            player.velocityY += gravity * deltaTime;
        }
        if (!zerogravity)
        {
            player1.velocityY += gravity * deltaTime;
        }
        float maxSpeedX = 600.0f;
        float maxSpeedY = 800.0f;

        if(player.velocityX > maxSpeedX) player.velocityX = maxSpeedX;
        if(player.velocityX < -maxSpeedX) player.velocityX = -maxSpeedX;

        if(player.velocityY > maxSpeedY) player.velocityY = maxSpeedY;
        if(player.velocityY < -maxSpeedY) player.velocityY = -maxSpeedY;



        player.transform.x += player.velocityX * deltaTime;
        player1.transform.x += player1.velocityX * deltaTime;
        

        for (const Block& block : blocks){
            
            float playerLeft   = player.transform.x - player.width / 2.0f;
            float playerRight  = player.transform.x + player.width / 2.0f;
            float playerTop    = player.transform.y - player.height / 2.0f;
            float playerBottom = player.transform.y + player.height / 2.0f;

            float blockLeft   = block.transform.x - block.width / 2.0f;
            float blockRight  = block.transform.x + block.width / 2.0f;
            float blockTop    = block.transform.y - block.height / 2.0f;
            float blockBottom = block.transform.y + block.height / 2.0f;

            float player1Left   = player1.transform.x - player1.width / 2.0f;
            float player1Right  = player1.transform.x + player1.width / 2.0f;
            float player1Top    = player1.transform.y - player1.height / 2.0f;
            float player1Bottom = player1.transform.y + player1.height / 2.0f;

            

            bool overlap =
                playerRight  > blockLeft  &&
                playerLeft   < blockRight &&
                playerBottom > blockTop   &&
                playerTop    < blockBottom;
            bool overlap1 =
                player1Right  > blockLeft  &&
                player1Left   < blockRight &&
                player1Bottom > blockTop   &&
                player1Top    < blockBottom;

            bool overlapplayer =
                playerRight > player1Left &&
                playerLeft < player1Right &&            
                playerBottom > player1Top &&
                playerTop < player1Bottom;

            if (overlap)
            {
                if (player.velocityX > 0)
                {
                    player.transform.x = blockLeft - player.width / 2.0f;
                }
                else if (player.velocityX < 0)
                {
                    player.transform.x = blockRight + player.width / 2.0f;
                }

                player.velocityX = 0;
            }
            if (overlap1)
            {
                if (player1.velocityX > 0)
                {
                    player1.transform.x = blockLeft - player1.width / 2.0f;
                }
                else if (player1.velocityX < 0)
                {
                    player1.transform.x = blockRight + player1.width / 2.0f;
                }

                player1.velocityX = 0;
            }
            
            if (overlapplayer)
            {
                if (player.velocityX > 0)
                {
                   player.transform.x = player1Left - player.width / 2.0f;
                }
                else if (player.velocityX < 0)
                {
                    player.transform.x = player1Right + player.width / 2.0f;
                }

                player.velocityX = 0;
            }
        }
        
        player.transform.y += player.velocityY * deltaTime;
        player1.transform.y += player1.velocityY * deltaTime;

        for (const Block& block : blocks){
            
            float playerLeft   = player.transform.x - player.width / 2.0f;
            float playerRight  = player.transform.x + player.width / 2.0f;
            float playerTop    = player.transform.y - player.height / 2.0f;
            float playerBottom = player.transform.y + player.height / 2.0f;

            float blockLeft   = block.transform.x - block.width / 2.0f;
            float blockRight  = block.transform.x + block.width / 2.0f;
            float blockTop    = block.transform.y - block.height / 2.0f;
            float blockBottom = block.transform.y + block.height / 2.0f;

            float player1Left   = player1.transform.x - player1.width / 2.0f;
            float player1Right  = player1.transform.x + player1.width / 2.0f;
            float player1Top    = player1.transform.y - player1.height / 2.0f;
            float player1Bottom = player1.transform.y + player1.height / 2.0f;

            

            bool overlap =
                playerRight  > blockLeft  &&
                playerLeft   < blockRight &&
                playerBottom > blockTop   &&
                playerTop    < blockBottom;

            bool overlapplayer =
                playerRight > player1Left &&
                playerLeft < player1Right &&            
                playerBottom > player1Top &&
                playerTop < player1Bottom;
            

            bool overlap1 =
                player1Right  > blockLeft  &&
                player1Left   < blockRight &&
                player1Bottom > blockTop   &&   
                player1Top    < blockBottom;

                   

            if(overlap)
            {   
                
                if (player.velocityY >= 0)
                {
                    player.transform.y = blockTop - player.height / 2.0f;
                    player.velocityY = 0;
                    isGrounded = true;
                } 
               
                else if (player.velocityY < 0)
                {
                    player.transform.y = blockBottom + player.height / 2.0f;
                    player.velocityY = 0;  
                }
                
                 
            }
            if(overlap1)
            {   
                
                if (player1.velocityY >= 0)
                {
                    player1.transform.y = blockTop - player1.height / 2.0f;
                    player1.velocityY = 0;
                    isGrounded = true;
                } 
               
                else if (player1.velocityY < 0)
                {
                    player1.transform.y = blockBottom + player1.height / 2.0f;
                    player1.velocityY = 0;  
                }
                
                 
            }

            if(overlapplayer)
            {   
                
                if (player.velocityY >= 0)
                {
                    player.transform.y = player1Top - player.height/2;
                    
                } 
               
                else if (player.velocityY < 0)
                {
                   player.transform.y = player1Top - player.height/2;
                    
                }
                player.velocityY = 0;  
                
                 
            }
       }
        
        float damping = 3.0f;
        player.velocityX -= player.velocityX * damping * deltaTime;
        player.velocityY -= player.velocityY * damping * deltaTime;
        player1.velocityX -= player1.velocityX * damping * deltaTime;
        player1.velocityY -= player1.velocityY * damping * deltaTime;
        if(player.transform.x < leftBoundary)
            player.transform.x = leftBoundary;
        if(player.transform.x > rightBoundary)
            player.transform.x = rightBoundary;
        if(player.transform.y < topBoundary)
        {
          player.transform.y = topBoundary;
          player.velocityY = 0;
        }
        if(player.transform.y > bottomBoundary)
        {
           player.transform.y = bottomBoundary;
           player.velocityY = 0;
           isGrounded = true;
        }
        if(player1.transform.x < leftBoundary)
            player1.transform.x = leftBoundary;
        if(player1.transform.x > rightBoundary)
            player1.transform.x = rightBoundary;
        if(player1.transform.y < topBoundary)
        {
          player1.transform.y = topBoundary;
          player1.velocityY = 0;
        }
        if(player1.transform.y > bottomBoundary)
        {
           player1.transform.y = bottomBoundary;
           player1.velocityY = 0;
           isGrounded = true;
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
        Rect.x = player.transform.x - player.width/2;
        Rect.y = player.transform.y - player.height/2;
        Rect.w = player.width;
        Rect.h = player.height;

        SDL_Rect enemyRect;
        enemyRect.x = player1.transform.x - player1.width/2;
        enemyRect.y = player1.transform.y - player1.height/2;
        enemyRect.w = player1.width;
        enemyRect.h = player1.height;

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