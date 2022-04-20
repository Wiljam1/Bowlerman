#include "player.h"
#include "game.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#define PUBLIC /* empty */
#define PRIVATE static

PRIVATE int HEIGHT = 300;
PRIVATE int WIDTH = 150;

PUBLIC void loadPlayer();
PUBLIC Player initPlayer(float xPos, float yPos);
PUBLIC void initPlayerRect(SDL_Rect *playerRect, Player player1);

struct playerController
{
    float xPos;
    float yPos;
    float speed;
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
    return HEIGHT;
}
PUBLIC int getPlayerWidth()
{
    return WIDTH;
}
PUBLIC float getPlayerXPosition(Player player)
{
    return player->xPos;
}
PUBLIC float getPlayerYPosition(Player player)
{
    return player->yPos;
}

// PUBLIC void destroyPlayer(Player player)
// {
    
// }
