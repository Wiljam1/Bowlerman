
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"
#include "game.h"

#define PUBLIC /* empty */
#define PRIVATE static

PRIVATE int PLAYERHEIGHT = 150;
PRIVATE int PLAYERWIDTH = 75;
PRIVATE int PLAYERSPEED = 300;     // speed in pixels/second


PUBLIC struct playerMovement;
PUBLIC struct playerController;

PUBLIC void loadPlayer();
PUBLIC Player initPlayer(float xPos, float yPos);
PUBLIC void initPlayerRect(SDL_Rect *playerRect, Player player1);


//struct for server
PUBLIC struct playerController
{
    int id;   //för packets via UDP behöver man kunna veta vem det är som skickar datan.
    float xPos;
    float yPos;
    float speed;
    char name[];
    
    //SDL_Rect playerRectangle;   //struct to hold the position and size of the sprite
};



PUBLIC SDL_Rect playerRect[4];

PUBLIC void initPlayerRect(SDL_Rect * playerRect, Player player1)
{
    playerRect->h = getPlayerHeight();
    playerRect->w = getPlayerWidth();
    playerRect->y = getPlayerYPosition(player1);
    playerRect->x = getPlayerYPosition(player1);
}


PUBLIC void resetPlayerMoving(PlayerMovement playerMoving)
{
    playerMoving->up=0;
    playerMoving->down=0;
    playerMoving->right=0;
    playerMoving->left=0;
    playerMoving->xPos=0;
    playerMoving->yPos=0;
    playerMoving->xPosOld=0;
    playerMoving->yPosOld=0;
}


PUBLIC Player initPlayer(float xPos, float yPos)
{
    Player p = malloc(sizeof(struct playerController));

    p->xPos = xPos;
    p->yPos = yPos;
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

PUBLIC void determinePlayerVelocity(PlayerMovement playerMoving)
{
    playerMoving->xVel=0;
    playerMoving->yVel=0;
    if(playerMoving->up && !playerMoving->down) playerMoving->yVel = -PLAYERSPEED;
    if(playerMoving->down && !playerMoving->up) playerMoving->yVel = PLAYERSPEED;
    if(playerMoving->left && !playerMoving->right) playerMoving->xVel = -PLAYERSPEED;
    if(playerMoving->right && !playerMoving->left) playerMoving->xVel = PLAYERSPEED;
}
PUBLIC void updatePlayerClientPosition(PlayerMovement playerMoving)
{
    playerMoving->xPos+= playerMoving->xVel / 60;
    playerMoving->yPos+= playerMoving->yVel / 60;
}

// PUBLIC void destroyPlayer(Player player)
// {
    
// }
