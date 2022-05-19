#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"
#include "game.h"
#include "collissionDetection.h"

#define MAXSPEED (WIDTH/265)
#define MINSPEED 1
#define MAXBOMBS 5
#define MAXPOWER 20
#define MAXLIVES 10
#define INVULNERABILITYTIME 3000

#define LEFT_X (WIDTH/17)
#define RIGHT_X (WIDTH/1.106)
#define TOP_Y (WIDTH/8.5)
#define BOTTOM_Y (WIDTH/1.368)

Uint32 pDeathCallback();

PUBLIC Player initPlayer(int xPos, int yPos, int playerID)
{
    //malloc(sizeof(struct playerController));
    int height = (WIDTH / 17);
    int width = (WIDTH / 26.56);
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
    p.speed = (double)MAXSPEED/2;
    p.speedDisplay = 1;
    p.amountOfBombs = 1;        //antal bomber, börjar på 1
    p.amountOfBombsPlaced = 0;  //antal bomber placerade, börjar på 0    
    p.explosionPower = 1;       //hur stor explosionen ska vara, börjar på 1
    p.canRollBombs = false;
    p.playerRect.h = p.height = height;
    p.playerRect.w = p.width = width;
    p.moveDirection = '0';
    p.isMoving = false;  //is not enforced by keyboard inputs though.
    p.isDead = false;
    p.isInvulnerable = false;
    p.noOfLives = 3;
    p.score = 0;

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
    for (int i = 0; i < theGame->playerAmount; i++)
        theGame->invulnerabiltyFlag[i] = false;
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
        playerRect[i].x = playerGetXPosition(theGame->player[i]);
        playerRect[i].y = playerGetYPosition(theGame->player[i]);
        playerRect[i].w = getPlayerWidth(theGame->player[i]);
        playerRect[i].h = getPlayerHeight(theGame->player[i]);
    }
    /*Managing sprite updates*/
    for(int i=0; i < theGame->playerAmount; i++)
    {
        if (theGame->player[i].isInvulnerable == false && theGame->player[i].noOfLives > 0){
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
        else if (theGame->player[i].noOfLives > 0 && theGame->player[i].isInvulnerable == true) {
            if (i % 2 == 0)
                SDL_RenderCopyEx(theGame->renderer, theGame->player_texture[i][spriteChoice[i]], &theGame->pSprites.BowlerManVert[0], &playerRect[i], 270, 0, 0);
            else
                SDL_RenderCopyEx(theGame->renderer, theGame->player_texture[i][spriteChoice[i]], &theGame->pSprites.BowlerManVert[0], &playerRect[i], 90, 0, 0);
            getStartPos(&theGame->player[i]); // If player is dead it respawns at starting pos
            theGame->player[i].isDead = false;
        }
        else {
            theGame->player[i].isDead = true;
            theGame->player[i].isInvulnerable = true;
        }
    }
}

PUBLIC void updateMovementDirection(Game theGame, int id, char c)
{
    theGame->player[id].moveDirection = c;
}

PUBLIC void updatePlayerXPosition(Game theGame, int id, double velX)
{
    theGame->player[id].xPos += velX;
}
PUBLIC void updatePlayerYPosition(Game theGame, int id, double velY)
{
    theGame->player[id].yPos += velY;
}
PUBLIC void playerSetXPos(Player *p, int x)
{
    p->xPos = x;
}
PUBLIC void playerSetYPos(Player *p, int y)
{
    p->yPos = y;
}

PUBLIC void playerSetOldXPos(Player *p, int x)
{
    p->xPosOld = x;
}

PUBLIC void playerSetOldYPos(Player *p, int y)
{
    p->yPosOld = y;
}

PUBLIC void playerSetMoveDirection(Player *p, char c)
{
    p->moveDirection = c;
}

PUBLIC void playerSetID(Player *p, int id)
{
    p->id = id;
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
PUBLIC float playerGetXPosition(Player p)
{
    return p.xPos;
}
PUBLIC float playerGetYPosition(Player p)
{
    return p.yPos;
}
PUBLIC double getPlayerSpeed(Player p)
{
    return p.speed;
}
PUBLIC int getLocalID(Game theGame)
{
    return theGame->playerIDLocal;
}

PUBLIC int playerGetNoOfLives(Player p)
{
    return p.noOfLives;
}

PUBLIC int playerGetOldXpos(Player p)
{
    return p.xPosOld;
}

PUBLIC int playerGetOldYPos(Player p)
{
    return p.yPosOld;
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
            p->xPos = LEFT_X;
            p->yPos = TOP_Y;
            break;
        case 1:
            p->xPos = RIGHT_X;
            p->yPos = TOP_Y;
            break;
        case 2:
            p->xPos = LEFT_X;
            p->yPos = BOTTOM_Y;
            break;
        case 3:
            p->xPos = RIGHT_X;
            p->yPos = BOTTOM_Y;
            break;
    }
}
PUBLIC int playerGetAmountOfBombsPlaced(Player p)
{
    return p.amountOfBombsPlaced;
}
PUBLIC int playerGetAmountOfBombs(Player p)
{
    return p.amountOfBombs;
}
PUBLIC int playerGetIsInvulnerable(Player p)
{
    return p.isInvulnerable;
}
PUBLIC int playerGetExplosionPower(Player p)
{
    return p.explosionPower;
}

void playerIncreaseSpeed(Player *p)
{
    playerAddSpeedDisplay(p, 1);
    printf("Adding speed: %lf\n", (double)MAXSPEED/10);
    p->speed += (double)MAXSPEED/8;
    if(p->speed > (double)MAXSPEED)
        p->speed = (double)MAXSPEED;
    if(p->speed < (double)MINSPEED)
        p->speed = (double)MINSPEED;
}
void playerAddSpeedDisplay(Player *p, int speedDisplay)
{
    p->speedDisplay += speedDisplay;
    if(p->speedDisplay > 5) 
        p->speedDisplay = 5;
}
void playerAddExplosionPower(Player *p, int explosionPower)
{
    p->explosionPower += explosionPower;
    if(p->explosionPower > MAXPOWER)
        p->explosionPower = MAXPOWER;
}
void playerAddAmountOfBombs(Player *p, int amountOfBombs)
{
    p->amountOfBombs += amountOfBombs;
    if(p->amountOfBombs > MAXBOMBS)
        p->amountOfBombs = MAXBOMBS;
}
void playerAddAmountOfBombsPlaced(Player *p, int i)
{
    p->amountOfBombsPlaced += i;
}
void playerAddLives(Player *p, int lives)
{
    p->noOfLives += lives;
    if(p->noOfLives > MAXLIVES)
        p->noOfLives = MAXLIVES;
}
void playerAddScore(Player *p, int score)
{
    p->score += score;
}

void playerDeathTimer(Game theGame)
{
        for (int i = 0; i < PLAYERAMOUNT; i++)
        {
            if (theGame->invulnerabiltyFlag[i] == true)
            {
                printf("Player %d invulnerability is: %s\n", i, theGame->player[i].isInvulnerable ? "On" : "Off");
                SDL_TimerID timerID = SDL_AddTimer(INVULNERABILITYTIME, pDeathCallback, (Game)theGame); // Funktionen körs efter antal ms som INVULTIME är satt till (ny tråd)
                if (!timerID) {
                    SDL_RemoveTimer(timerID);
                    printf("Timer failed\n");
                }
                else {
                    printf("Timer id for player %d: %d\n", i, timerID);
                }
                theGame->invulnerabiltyFlag[i] = false;
            }
        }
}

Uint32 pDeathCallback(Uint32 interval, Game theGame)
{
    for (int i = 0; i < PLAYERAMOUNT; i++)
    {
        if(theGame->player[i].isInvulnerable == true)
        {
            theGame->player[i].isInvulnerable = false;
            printf("Player %d invulnerability is: %s\n", i, theGame->player[i].isInvulnerable ? "On" : "Off");
        }
    }
    return 0;
}

void setPlayerDeathFlags(Game theGame, int i)
{
    theGame->updateFlag = true;
    theGame->player[i].noOfLives--;
    printf("Lives left for player %d: %d\n", i, theGame->player[i].noOfLives);
    theGame->player[i].isDead = true;
    theGame->player[i].isInvulnerable = true;
    theGame->invulnerabiltyFlag[i] = true; /*Flagga för att inte komma in i timern mer en än gång*/
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
