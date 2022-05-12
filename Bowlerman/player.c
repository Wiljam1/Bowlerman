#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"
#include "game.h"
#include "collissionDetection.h"

#define MAXSPEED 8
#define MINSPEED 1
#define MAXBOMBS 5

PUBLIC Player initPlayer(int xPos, int yPos, int playerID)
{
    //malloc(sizeof(struct playerController));

    Player p;

    p.id = playerID;
    p.up = 0;
    p.down = 0;
    p.right = 0;
    p.left = 0;
    p.xVel = 0;
    p.yVel = 0;
    p.playerRect.x = p.xPos = p.xPosOld = xPos; // Startvärden för x och y ges till alla variabler.
    p.playerRect.y = p.yPos = p.yPosOld = yPos;
    p.speed = 4;
    p.amountOfBombs = 1;        //antal bomber, börjar på 1
    p.amountOfBombsPlaced = 0;  //antal bomber placerade, börjar på 0    
    p.explosionPower = 1;       //hur stor explosionen ska vara, börjar på 1
    p.canRollBombs = false;
    p.playerRect.h = p.height = 100*0.7;
    p.playerRect.w = p.width = 64*0.7;
    p.moveDirection = '0';
    p.isMoving=false;  //is not enforced by keyboard inputs though.
    p.isDead = false;

    return p;
}

void initAllPlayers(Game theGame)
{
    // init x amount of players
    theGame->player[0] = initPlayer(LEFT_X, TOP_Y, 0); // sets x and y coordinates and resets values.

    if (theGame->playerAmount > 1)
    {
        theGame->player[1] = initPlayer(RIGHT_X, TOP_Y, 1); // sets x and y coordinates and resets values.
    }
    if (theGame->playerAmount > 2)
    {
        theGame->player[2] = initPlayer(LEFT_X, BOTTOM_Y, 2); // sets x and y coordinates and resets values.
    }
    if (theGame->playerAmount > 3)
    {
        theGame->player[3] = initPlayer(RIGHT_X, BOTTOM_Y, 3); // sets x and y coordinates and resets values.
    }
}

void UpdatePlayerTextures(Game theGame)
{
    /*Keeps track of what sprites to load and at what timing*/
    static Uint8 updateSprite[4] = {0};
    static Uint8 spriteTimer[4] = {0};
    Uint8 spriteChoice[4] = {0}; 
    
    /*Init all player rectangles*/
    SDL_Rect playerRect[4];
    for(int i=0; i<theGame->playerAmount; i++)
    {
        playerRect[i].x = theGame->player[i].xPos;
        playerRect[i].y = theGame->player[i].yPos;
        playerRect[i].w= theGame->player->width;
        playerRect[i].h = theGame->player->height;
    }
    /*Managing sprite updates*/
    for(int i=0; i < theGame->playerAmount; i++)
    {
        if (theGame->player[i].isDead == false){
            if (spriteTimer[i] > 10){ //Slowing down sprite updates
                spriteTimer[i] = 0; 
            }
                switch (theGame->player[i].moveDirection)
                {
                    case 'w': 
                        spriteChoice[i] = 1;
                        SDL_RenderCopy(theGame->renderer, theGame->player_texture[i][spriteChoice[i]], &theGame->pSprites.BowlerManVert[updateSprite[i]], &playerRect[i]);
                        break;
                    case 'a': spriteChoice[i] = 3;
                        SDL_RenderCopy(theGame->renderer, theGame->player_texture[i][spriteChoice[i]], &theGame->pSprites.BowlerManHori[updateSprite[i]], &playerRect[i]);
                        break;
                    case 's': spriteChoice[i] = 0;
                        SDL_RenderCopy(theGame->renderer, theGame->player_texture[i][spriteChoice[i]], &theGame->pSprites.BowlerManVert[updateSprite[i]], &playerRect[i]);
                        break;
                    case 'd': spriteChoice[i] = 2;
                        SDL_RenderCopy(theGame->renderer, theGame->player_texture[i][spriteChoice[i]], &theGame->pSprites.BowlerManHori[updateSprite[i]], &playerRect[i]);
                        break;
                    case '0':
                    default: spriteChoice[i] = 0;
                        SDL_RenderCopy(theGame->renderer, theGame->player_texture[i][spriteChoice[i]], &theGame->pSprites.BowlerManVert[0], &playerRect[i]);
                        break;
                }
                if (spriteTimer[i]++ % 5 == 0 && theGame->player[i].moveDirection != '0'){ // Slowing down sprite updates
                        updateSprite[i]++;
                }
                if (updateSprite[i] > 7){
                        updateSprite[i] = 0;
                }
        }
        else{
            getStartPos(&theGame->player[i]); // If player is dead it respawns at starting pos
            theGame->player[i].isDead = false;
        }
    }
}

PlayerSprites GetPlayerSprite()
{
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

PUBLIC void updateMovementDirection(Game theGame, int id, char c)
{
    theGame->player[id].moveDirection = c;
}

PUBLIC void updatePlayerXPosition(Game theGame, int id, int velX)
{
    theGame->player[id].xPos += velX;
}
PUBLIC void updatePlayerYPosition(Game theGame, int id, int velY)
{
    theGame->player[id].yPos += velY;
}

PUBLIC char getMoveDirection(Player p)
{
    return p.moveDirection;
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
PUBLIC int getLocalID(Game theGame)
{
    return theGame->playerIDLocal;
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
void playerAddSpeed(Player *p, float speed)
{
    p->speed += speed;
    if(p->speed > MAXSPEED) 
        p->speed = MAXSPEED;
    if(p->speed < MINSPEED)
        p->speed = MINSPEED;
}
void playerAddExplosionPower(Player *p, int explosionPower)
{
    p->explosionPower += explosionPower;
}
void playerAddAmountOfBombs(Player *p, int amountOfBombs)
{
    p->amountOfBombs += amountOfBombs;
    if(p->amountOfBombs > MAXBOMBS)
        p->amountOfBombs = MAXBOMBS;
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
