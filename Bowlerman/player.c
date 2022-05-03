#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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
    p.playerRect.x = p.xPos = p.xPosOld = xPos;
    p.playerRect.y = p.yPos = p.yPosOld = yPos;
    p.speed = 4;
    p.playerRect.h = p.height = 100*0.7;
    p.playerRect.w = p.width = 64*0.7;
    p.moveDirection = '0';

    return p;
}

PlayerSprites GetPlayerSprite()
{
    // Ska göra lite for-loopar här
    PlayerSprites p;
    p.BowlerManVert[0].h = 100;
    p.BowlerManVert[0].w = 64;
    p.BowlerManVert[0].x = 0;
    p.BowlerManVert[0].y = 0;

    p.BowlerManVert[1].h = 100;
    p.BowlerManVert[1].w = 64;
    p.BowlerManVert[1].x = 64;
    p.BowlerManVert[1].y = 0;

    p.BowlerManVert[2].h = 100;
    p.BowlerManVert[2].w = 64;
    p.BowlerManVert[2].x = 128;
    p.BowlerManVert[2].y = 0;

    p.BowlerManVert[3].h = 100;
    p.BowlerManVert[3].w = 64;
    p.BowlerManVert[3].x = 192;
    p.BowlerManVert[3].y = 0;

    p.BowlerManVert[4].h = 100;
    p.BowlerManVert[4].w = 64;
    p.BowlerManVert[4].x = 256;
    p.BowlerManVert[4].y = 0;

    p.BowlerManVert[5].h = 100;
    p.BowlerManVert[5].w = 64;
    p.BowlerManVert[5].x = 320;
    p.BowlerManVert[5].y = 0;

    p.BowlerManVert[6].h = 100;
    p.BowlerManVert[6].w = 64;
    p.BowlerManVert[6].x = 384;
    p.BowlerManVert[6].y = 0;

    p.BowlerManVert[7].h = 100;
    p.BowlerManVert[7].w = 64;
    p.BowlerManVert[7].x = 448;
    p.BowlerManVert[7].y = 0;

    p.BowlerManHori[0].h = 100;
    p.BowlerManHori[0].w = 56;
    p.BowlerManHori[0].x = 0;
    p.BowlerManHori[0].y = 0;

    p.BowlerManHori[1].h = 100;
    p.BowlerManHori[1].w = 57;
    p.BowlerManHori[1].x = 56;
    p.BowlerManHori[1].y = 0;

    p.BowlerManHori[2].h = 100;
    p.BowlerManHori[2].w = 58;
    p.BowlerManHori[2].x = 113;
    p.BowlerManHori[2].y = 0;

    p.BowlerManHori[3].h = 100;
    p.BowlerManHori[3].w = 57;
    p.BowlerManHori[3].x = 171;
    p.BowlerManHori[3].y = 0;

    p.BowlerManHori[4].h = 100;
    p.BowlerManHori[4].w = 56;
    p.BowlerManHori[4].x = 228;
    p.BowlerManHori[4].y = 0;

    p.BowlerManHori[5].h = 100;
    p.BowlerManHori[5].w = 57;
    p.BowlerManHori[5].x = 284;
    p.BowlerManHori[5].y = 0;

    p.BowlerManHori[6].h = 100;
    p.BowlerManHori[6].w = 60;
    p.BowlerManHori[6].x = 341;
    p.BowlerManHori[6].y = 0;

    p.BowlerManHori[7].h = 100;
    p.BowlerManHori[7].w = 57;
    p.BowlerManHori[7].x = 401;
    p.BowlerManHori[7].y = 0;
    
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
PUBLIC int getPlayerSpeed(Player p)
{
    return p.speed;
}

// REplaced by different movement-implementation

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
