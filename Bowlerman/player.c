#include "player.h"
#include "game.h"
#include <stdlib.h>

#define PUBLIC /* empty */
#define PRIVATE static

PRIVATE int HEIGHT = 300;
PRIVATE int WIDTH = 150;

PUBLIC void loadPlayer();

struct playerController
{
    float xPos;
    float yPos;
    float speed;
};

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

// PUBLIC void destroyPlayer(Player player)
// {
    
// }
