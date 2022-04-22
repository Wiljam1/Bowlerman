#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"
#include "game.h"
#include "collissionDetection.h"

#define PUBLIC /* empty */
#define PRIVATE static

PRIVATE int PLAYERHEIGHT = 100;
PRIVATE int PLAYERWIDTH = 50;
PRIVATE int PLAYERSPEED = 300;     // speed in pixels/second


PUBLIC void loadPlayer();
PUBLIC Player initPlayer(float xPos, float yPos);
PUBLIC void initPlayerRect(SDL_Rect *playerRect, Player player1);






PUBLIC SDL_Rect playerRect[4];

PUBLIC void initPlayerRect(SDL_Rect * playerRect, Player player1)
{
    playerRect->h = getPlayerHeight();
    playerRect->w = getPlayerWidth();
    playerRect->y = getPlayerYPosition(player1);
    playerRect->x = getPlayerXPosition(player1);
}


PUBLIC Player initPlayer(float xPos, float yPos)
{
    Player p = malloc(sizeof(struct playerController));

    p->up=0;
    p->down=0;
    p->right=0;
    p->left=0;
    p->xVel=0;
    p->yVel=0;
    p->xPos = p->xPosOld = xPos;
    p->yPos = p->yPosOld = yPos;
    p->speed = 1;

    return p;
}


PUBLIC int getPlayerHeight()
{
    return PLAYERHEIGHT;
}
PUBLIC int getPlayerWidth()
{
    return PLAYERWIDTH;
}
PUBLIC float getPlayerXPosition(Player player)
{
    return player->xPos;
}
PUBLIC float getPlayerYPosition(Player player)
{
    return player->yPos;
}

PUBLIC void determinePlayerVelocity(Player playerMoving)
{
    playerMoving->xVel=0;
    playerMoving->yVel=0;
    if(playerMoving->up && !playerMoving->down) playerMoving->yVel = -PLAYERSPEED;
    if(playerMoving->down && !playerMoving->up) playerMoving->yVel = PLAYERSPEED;
    if(playerMoving->left && !playerMoving->right) playerMoving->xVel = -PLAYERSPEED;
    if(playerMoving->right && !playerMoving->left) playerMoving->xVel = PLAYERSPEED;
}
PUBLIC void updatePlayerClientPosition(Player playerMoving)
{
    playerMoving->xPos+= playerMoving->xVel / 60;
    playerMoving->yPos+= playerMoving->yVel / 60;
}

// PUBLIC void destroyPlayer(Player player)
// {
    
// }
