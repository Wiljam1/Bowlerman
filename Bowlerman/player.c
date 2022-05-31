#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"
#include "Game.h"
#include "collisionDetection.h"

#define MAXSPEED (WIDTH/265)
#define MINSPEED 1
#define MAXBOMBS 5
#define MAXPOWER 20
#define MAXLIVES 10
#define INVULNERABILITYTIME 3000
struct playerController
{
    int id;   //för packets via UDP behöver man kunna veta vem det är som skickar datan.
    bool up, down, left, right;
    float yVel, xVel, xPos, yPos;
    float xPosOld;
    float yPosOld;
    bool isMoving; //is the player moving or not

    double speed;
    int amountOfBombs, explosionPower, amountOfBombsPlaced, speedDisplay;
    bool canRollBombs;

    int height;
    int width;
    char name[40];
    bool isDead, isInvulnerable;
    SDL_Rect playerRect;   //struct to hold the position and size of the sprite
    char moveDirection;  // Keeps track of player movement direction for sprite rendering
    int noOfLives, score;

    int noOfPlayers;
}; 

Uint32 pDeathCallback();

PUBLIC Player initPlayer(int initX, int initY, int playerID)
{
    int height = (WIDTH / 17);
    int width = (WIDTH / 26.56);
    Player p = malloc(sizeof(struct playerController));

    p->id = playerID;
    p->up = 0;
    p->down = 0;
    p->right = 0;
    p->left = 0;
    p->xVel = 0;
    p->yVel = 0;
    p->playerRect.x = p->xPos = p->xPosOld = initX; // Startvärden för x och y ges till alla variabler.
    p->playerRect.y = p->yPos = p->yPosOld = initY;
    p->speed = (double)MAXSPEED/2;
    p->speedDisplay = 1;
    p->amountOfBombs = 1;        //antal bomber, börjar på 1
    p->amountOfBombsPlaced = 0;  //antal bomber placerade, börjar på 0    
    p->explosionPower = 1;       //hur stor explosionen ska vara, börjar på 1
    p->canRollBombs = false;
    p->playerRect.h = p->height = height;
    p->playerRect.w = p->width = width;
    p->moveDirection = '0';
    p->isMoving = false;  //is not enforced by keyboard inputs though.
    p->isDead = false;
    p->isInvulnerable = false;
    p->noOfLives = 3; // OM du ändrar här måste du ändra till samma i UDPDataReset!!
    p->score = 0;

    p->noOfPlayers = 4;     // Spelarantal för diverse funktioner

    return p;
}

void initAllPlayers(Game theGame, Player player[])
{
    player[0] = initPlayer(LEFT_X, TOP_Y, 0); // Init all possible players positions and IDs.
    player[1] = initPlayer(RIGHT_X, TOP_Y, 1);
    player[2] = initPlayer(LEFT_X, BOTTOM_Y, 2);
    player[3] = initPlayer(RIGHT_X, BOTTOM_Y, 3);
    printf("Player Amount is: %d\n", gameGetPlayerAmount(theGame));

    int i;
    for (i = 0; i < PLAYERAMOUNT; i++) {        // Init all possible players to dead
        gameSetInvulFlag(theGame, i, true);
        playerSetInvulnerability(player, i, true);
        playerSetDead(player, i);
        playerSetPlayerCount(player, theGame->playerAmount, i); // Setting the player count in the player struct for other functions.
    }
    for (i = 0; i < theGame->playerAmount; i++) {   // Setting the actual amount of players to alive
        gameSetInvulFlag(theGame, i, false);
        playerSetInvulnerability(player, i, false);
        playerSetAlive(player, i);
    }
}

void manageMovementInputs(Game theGame, Player player[])
{
    double velX = 0, velY = 0;
    int id = gameGetLocalID(theGame);
    char direction;
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (!playerGetIsDead(player, id))
    {
        if (state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_S])
        {
            velX = -getPlayerSpeed(player, id);
            direction = 'a';
        }
        else if (state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_S])
        {
            velX = getPlayerSpeed(player, id);
            direction = 'd';
        }
        if (velX == 0)
        {
            if (state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_S])
            {
                velY = -getPlayerSpeed(player, id);
                direction = 'w';
            }
            else if (state[SDL_SCANCODE_S] && !state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_D])
            {
                velY = getPlayerSpeed(player, id);
                direction = 's';
            }
        }
    }
    if (!velX && !velY) {
        direction = '0';
    }
    // Update player positions
    playerSetMoveDirection(player, id, direction);
    updatePlayerXPosition(player, id, velX);
    updatePlayerYPosition(player, id, velY);
}


void UpdatePlayerTextures(Game theGame, Player player[])
{
    /*Keeps track of what sprites to load and at what timing*/
    static Uint8 updateSprite[4] = {0};
    static Uint8 spriteTimer[4] = {0};
    Uint8 spriteChoice[4] = {0};
    
    /*Init all player rectangles*/
    SDL_Rect playerRect[4];
    for(int i=0; i < gameGetPlayerAmount(theGame); i++)
    {
        playerRect[i].x = playerGetXPosition(player, i);
        playerRect[i].y = playerGetYPosition(player, i);
        playerRect[i].w = playerGetWidth(player, i);
        playerRect[i].h = playerGetHeight(player, i);
    }
    /*Managing sprite updates*/
    for(int i=0; i < gameGetPlayerAmount(theGame); i++)
    {
        if (playerGetIsInvulnerable(player, i) == false && playerGetNoOfLives(player, i) > 0){
            if (spriteTimer[i] > 10){ //Slowing down sprite updates
                spriteTimer[i] = 0; 
            }
                switch (playerGetMoveDirection(player, i))
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
                if (spriteTimer[i]++ % 5 == 0 && playerGetMoveDirection(player, i) != '0'){ // Slowing down sprite updates
                        updateSprite[i]++;
                }
                if (updateSprite[i] > 7){
                        updateSprite[i] = 0;
                }
        }
        else if (playerGetNoOfLives(player, i) > 0 && playerGetIsInvulnerable(player, i) == true) {
            static int rotate = 90;
            rotate += 10;
            if(rotate > 360)
                rotate = 0;
            SDL_RenderCopyEx(theGame->renderer, theGame->player_texture[i][spriteChoice[i]], &theGame->pSprites.BowlerManVert[0], &playerRect[i], rotate, 0, 0);
        }
        else if (playerGetNoOfLives(player, i) <= 0)
        {
            playerSetDead(player, i);
            playerSetInvulnerability(player, i, true);
        }
    }
}

PUBLIC void playerSetDead(Player p[], int id)
{
    p[id]->isDead = true;
}

PUBLIC void playerSetAlive(Player p[], int id)
{
    p[id]->isDead = false;
}

PUBLIC bool playerGetDeadorAlive(Player p[], int id)
{
    return p[id]->isDead;
}

PUBLIC void playerSetMoveDirection(Player p[], int id, char c)
{
    p[id]->moveDirection = c;
}

PUBLIC void updatePlayerXPosition(Player p[], int id, double velX)
{
    p[id]->xPos += velX;
}
PUBLIC void updatePlayerYPosition(Player p[], int id, double velY)
{
    p[id]->yPos += velY;
}
PUBLIC void playerSetXPos(Player p[], int id, float x)
{
    p[id]->xPos = x;
}
PUBLIC void playerSetYPos(Player p[], int id, float y)
{
    p[id]->yPos = y;
}

PUBLIC void playerSetOldXPos(Player p[], int id, float x)
{
    p[id]->xPosOld = x;
}

PUBLIC void playerSetOldYPos(Player p[], int id, float y)
{
    p[id]->yPosOld = y;
}
PUBLIC void playerSetPlayerCount(Player *p, int n, int id)
{
    p[id]->noOfPlayers = n;
}

PUBLIC int playerGetPlayerCount(Player *p, int id)
{
    return p[id]->noOfPlayers;
}

PUBLIC void playerSetID(Player p[], int id)
{
    p[id]->id = id;
}

PUBLIC char playerGetMoveDirection(Player p[], int id)
{
    return p[id]->moveDirection;
}

PUBLIC int playerGetHeight(Player p[], int id)
{
    return p[id]->height;
}
PUBLIC int playerGetWidth(Player p[], int id)
{
    return p[id]->width;
}
PUBLIC float playerGetXPosition(Player p[], int id)
{
    return p[id]->xPos;
}
PUBLIC float playerGetYPosition(Player p[], int id)
{
    return p[id]->yPos;
}
PUBLIC double getPlayerSpeed(Player p[], int id)
{
    return p[id]->speed;
}

PUBLIC int playerGetNoOfLives(Player p[], int id)
{
    return p[id]->noOfLives;
}

PUBLIC void playerSetNoOfLives(Player p[], int id, int lives)
{
    p[id]->noOfLives = lives;
}

PUBLIC int playerGetOldXpos(Player p[], int id)
{
    return p[id]->xPosOld;
}

PUBLIC int playerGetOldYPos(Player p[], int id)
{
    return p[id]->yPosOld;
}

PUBLIC int getPlayerID(Player p[], int id)
{
    return p[id]->id;
}
PUBLIC int playerGetScore(Player p[], int id)
{
    return p[id]->score;
}

PUBLIC void playerSetScore(Player p[], int id, int score)
{
    p[id]->score = score;
}

PUBLIC int playerGetSpeedDisplay(Player p[], int id)
{
    return p[id]->speedDisplay;
}

PUBLIC int playerGetAmountOfBombsPlaced(Player p[], int id)
{
    return p[id]->amountOfBombsPlaced;
}
PUBLIC int playerGetAmountOfBombs(Player p[], int id)
{
    return p[id]->amountOfBombs;
}
PUBLIC int playerGetIsInvulnerable(Player p[], int id)
{
    return p[id]->isInvulnerable;
}

PUBLIC void playerSetInvulnerability(Player p[], int id, bool b)
{
    p[id]->isInvulnerable = b;
}

PUBLIC int playerGetExplosionPower(Player p[], int id)
{
    return p[id]->explosionPower;
}

int playerGetIsDead(Player p[], int id)
{
    return p[id]->isDead;
}

void playerIncreaseSpeed(Player p[], int id)
{
    playerAddSpeedDisplay(p, 1, id);
    p[id]->speed += (double)MAXSPEED/8;
    if(p[id]->speed > (double)MAXSPEED)
        p[id]->speed = (double)MAXSPEED;
    if(p[id]->speed < (double)MINSPEED)
        p[id]->speed = (double)MINSPEED;
}
void playerAddSpeedDisplay(Player p[], int speedDisplay, int id)
{
    p[id]->speedDisplay += speedDisplay;
    if(p[id]->speedDisplay > 5) 
        p[id]->speedDisplay = 5;
}
void playerAddExplosionPower(Player p[], int id, int explosionPower)
{
    p[id]->explosionPower += explosionPower;
    if(p[id]->explosionPower > MAXPOWER)
        p[id]->explosionPower = MAXPOWER;
}
void playerAddAmountOfBombs(Player p[], int id, int amountOfBombs)
{
    p[id]->amountOfBombs += amountOfBombs;
    if(p[id]->amountOfBombs > MAXBOMBS)
        p[id]->amountOfBombs = MAXBOMBS;
}
void playerAddAmountOfBombsPlaced(Player p[], int id, int i)
{
    p[id]->amountOfBombsPlaced += i;
}
void playerAddLives(Player p[], int id, int lives)
{
    p[id]->noOfLives += lives;
    if(p[id]->noOfLives > MAXLIVES)
        p[id]->noOfLives = MAXLIVES;
}
void playerAddScore(Player p[], int id, int score)
{
    p[id]->score += score;
}

void playerDeathTimer(Game theGame, Player player[])
{
        for (int i = 0; i < playerGetPlayerCount(player, i); i++)
        {
            if (gameGetInvulFlag(theGame, i))
            {
                printf("Player %d invulnerability is: %s\n", i, playerGetIsInvulnerable(player, i) ? "On" : "Off");
                SDL_TimerID timerID = SDL_AddTimer(INVULNERABILITYTIME, pDeathCallback, player); // Funktionen körs efter antal ms som INVULTIME är satt till (ny tråd)
                if (!timerID) {
                    SDL_RemoveTimer(timerID);
                    printf("Timer failed\n");
                }
                else {
                    printf("Timer id for player %d: %d\n", i, timerID);
                }
                gameSetInvulFlag(theGame, i, false);
            }
        }
}

Uint32 pDeathCallback(Uint32 interval, Player player[])
{
    for (int i = 0; i < playerGetPlayerCount(player, i); i++)
    {
        if(playerGetIsInvulnerable(player, i) == true)
        {
            playerSetInvulnerability(player, i, false);
            printf("Player %d invulnerability is: %s\n", i, playerGetIsInvulnerable(player, i) ? "On" : "Off");
        }
    }
    return 0;
}

void setPlayerDeathFlags(Game theGame, Player player[], int i)
{
    gameUpdateFlagSet(theGame, true);
    int lives = playerGetNoOfLives(player, i);
    if(lives > 0) lives--;
    playerSetNoOfLives(player, i, lives);
    printf("Lives left for player %d: %d\n", i, playerGetNoOfLives(player, i));
    if(lives == 0) playerSetDead(player, i);
    playerSetInvulnerability(player, i , true);
    gameSetInvulFlag(theGame, i, true); /*Flagga för att inte komma in i timern mer en än gång*/
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
//     p[id]->xVel=0;
//     p[id]->yVel=0;
//     if(p[id]->up && !p[id]->down) p[id]->yVel = -p[id]->speed;
//     if(p[id]->down && !p[id]->up) p[id]->yVel = p[id]->speed;
//     if(p[id]->left && !p[id]->right) p[id]->xVel = -p[id]->speed;
//     if(p[id]->right && !p[id]->left) p[id]->xVel = p[id]->speed;
// }
// PUBLIC void updatePlayerClientPosition(Player p)
// {
//     p[id]->xPos += p[id]->xVel;
//     p[id]->yPos += p[id]->yVel;
// }

// PUBLIC void destroyPlayer(Player player)
// {

// }
