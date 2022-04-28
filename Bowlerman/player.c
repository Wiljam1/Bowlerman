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
    p.speed = 1;
    p.playerRect.h = p.height = 100;
    p.playerRect.w = p.width = 80;
    p.moveDirection = '0';

    return p;
}

PlayerSprites GetPlayerSprite()
{
    PlayerSprites p;
    const int width = 184;
    const int baseX = 43;
    const int baseY = 20;
    const int height = 307;

    p.BowlerMan[0].h = height;
    p.BowlerMan[0].w = width;
    p.BowlerMan[0].x = baseX;
    p.BowlerMan[0].y = 0;

    p.BowlerMan[1].h = height;
    p.BowlerMan[1].w = width;
    p.BowlerMan[1].x = baseX * 2 + width + 32;
    p.BowlerMan[1].y = 0;

    p.BowlerMan[2].h = height;
    p.BowlerMan[2].w = width;
    p.BowlerMan[2].x = baseX * 4 + width * 2 + 21;
    p.BowlerMan[2].y = 0;

    p.BowlerMan[3].h = height;
    p.BowlerMan[3].w = width;
    p.BowlerMan[3].x = baseX;
    p.BowlerMan[3].y = height;

    p.BowlerMan[4].h = height;
    p.BowlerMan[4].w = width;
    p.BowlerMan[4].x = baseX * 2 + width + 32;
    p.BowlerMan[4].y = height;

    p.BowlerMan[5].h = height;
    p.BowlerMan[5].w = width;
    p.BowlerMan[5].x = baseX * 4 + width * 2 + 21;
    p.BowlerMan[5].y = height;

    p.BowlerMan[6].h = height;
    p.BowlerMan[6].w = width;
    p.BowlerMan[6].x = baseX;
    p.BowlerMan[6].y = height * 2;

    p.BowlerMan[7].h = height;
    p.BowlerMan[7].w = width;
    p.BowlerMan[7].x = baseX * 2 + width + 32;
    p.BowlerMan[7].y = height * 2;

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
