#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"
#include "game.h"
#include "collissionDetection.h"

PUBLIC Player initPlayer(float xPos, float yPos)
{
    malloc(sizeof(struct playerController));

    Player p;

    p.up = 0;
    p.down = 0;
    p.right = 0;
    p.left = 0;
    p.xVel = 0;
    p.yVel = 0;
    p.xPos = p.xPosOld = xPos;
    p.yPos = p.yPosOld = yPos;
    p.speed = 3;
    p.height = 80;
    p.width = 50;

    return p;
}


PUBLIC int getPlayerHeight(Player p)
{
    return p.height;
}
PUBLIC int getPlayerWidth(Player p)
{
    return p.width;
}
PUBLIC float getPlayerXPosition(Player p)
{
    return p.xPos;
}
PUBLIC float getPlayerYPosition(Player p)
{
    return p.yPos;
}

//REplaced by different movement-implementation

// PUBLIC void determinePlayerVelocity(Player p)
// {
//     p->xVel=0;
//     p->yVel=0;
//     if(p->up && !p->down) p->yVel = -p->speed;
//     if(p->down && !p->up) p->yVel = p->speed;
//     if(p->left && !p->right) p->xVel = -p->speed;
//     if(p->right && !p->left) p->xVel = p->speed;
// }
// PUBLIC void updatePlayerClientPosition(Player p)
// {
//     p->xPos += p->xVel;
//     p->yPos += p->yVel;
// }

// PUBLIC void destroyPlayer(Player player)
// {
    
// }
