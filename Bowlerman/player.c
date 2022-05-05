#include "player.h"

PUBLIC Player initPlayer(int xPos, int yPos, int playerID)
{
    malloc(sizeof(struct playerController));

    Player p;

    p.id = playerID;
    p.up = 0;
    p.down = 0;
    p.right = 0;
    p.left = 0;
    p.xVel = 0;
    p.yVel = 0;
    p.playerRect.x = p.xPos = p.xPosOld = xPos;
    p.playerRect.y = p.yPos = p.yPosOld = yPos;
    p.speed = 4;
    p.playerRect.h = p.height = 70;
    p.playerRect.w = p.width = 45;
    p.moveDirection = '0';
    p.isDead = false;

    return p;
}

PlayerSprites GetPlayerSprite()
{
    // Ska göra lite for-loopar här
    PlayerSprites p;

    for(int i = 0; i < 8; i++){
        p.BowlerManVert[i].h = 100;
        p.BowlerManVert[i].w = 64;
        p.BowlerManVert[i].y = 0;
        p.BowlerManVert[i].x = i * 64;
        p.BowlerManHori[i].h = 100;
        p.BowlerManHori[i].y = 0;
    }

    p.BowlerManHori[0].w = 56;
    p.BowlerManHori[0].x = 0;

    p.BowlerManHori[1].w = 57;
    p.BowlerManHori[1].x = 56;

    p.BowlerManHori[2].w = 58;
    p.BowlerManHori[2].x = 113;

    p.BowlerManHori[3].w = 57;
    p.BowlerManHori[3].x = 171;

    p.BowlerManHori[4].w = 56;
    p.BowlerManHori[4].x = 228;

    p.BowlerManHori[5].w = 57;
    p.BowlerManHori[5].x = 284;

    p.BowlerManHori[6].w = 60;
    p.BowlerManHori[6].x = 341;

    p.BowlerManHori[7].w = 57;
    p.BowlerManHori[7].x = 401;
    
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
PUBLIC int getPlayerID(Player p)
{
    return p.id;
}
PUBLIC void getStartPos(Player *p)
{
    switch (p->id)
    {
        case 0:
            p->xPos = 70;
            p->yPos = 140;
            break;
        case 1:
            p->xPos = 1076;
            p->yPos = 140;
            break;
        case 2:
            p->xPos = 70;
            p->yPos = 870;
            break;
        case 3:
            p->xPos = 1076;
            p->yPos = 870;
            break;
    }
}
