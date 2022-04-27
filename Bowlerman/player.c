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
    p.playerRect.h = p.height = 100;
    p.playerRect.w = p.width = 80;

    return p;
}

PlayerSprites getRedPlayer()
{
    PlayerSprites p;
    const int width = 184;
    const int baseX = 43;
    const int baseY = 20;
    const int height = 307;

    p.redMan[0][0].h = height;
    p.redMan[0][0].w = width;
    p.redMan[0][0].x = baseX;
    p.redMan[0][0].y = 0;

    p.redMan[0][1].h = height;
    p.redMan[0][1].w = width;
    p.redMan[0][1].x = baseX * 2 + width + 32;
    p.redMan[0][1].y = 0;

    p.redMan[0][2].h = height;
    p.redMan[0][2].w = width;
    p.redMan[0][2].x = baseX * 4 + width * 2 + 21;
    p.redMan[0][2].y = 0;

    p.redMan[0][3].h = height;
    p.redMan[0][3].w = width;
    p.redMan[0][3].x = baseX;
    p.redMan[0][3].y = height;

    p.redMan[0][4].h = height;
    p.redMan[0][4].w = width;
    p.redMan[0][4].x = baseX * 2 + width + 32;
    p.redMan[0][4].y = height;

    p.redMan[0][5].h = height;
    p.redMan[0][5].w = width;
    p.redMan[0][5].x = baseX * 4 + width * 2 + 21;
    p.redMan[0][5].y = height;

    p.redMan[0][6].h = height;
    p.redMan[0][6].w = width;
    p.redMan[0][6].x = baseX;
    p.redMan[0][6].y = height * 2;

    p.redMan[0][7].h = height;
    p.redMan[0][7].w = width;
    p.redMan[0][7].x = baseX * 2 + width + 32;
    p.redMan[0][7].y = height * 2;

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
