#include "powerup.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include "player.h"
#include "game.h"

void rollForPowerup(Game theGame, int xPos, int yPos)
{
    static int currentPowerup = 0;    //Kanske vill göra på ett annat sätt här
    if(currentPowerup == 0)
    {
        currentPowerup = theGame->playerIDLocal;
    }
    if((rand() % 100+1) < 40){ // 40% chance to spawn a powerup                                                      
        theGame->powerups[currentPowerup] = powerupPlace(xPos, yPos, rand()%POWERUPTYPES); //Type = random number between 0 and how many types there are
        theGame->powerups[currentPowerup].id = currentPowerup;
        theGame->powerups[currentPowerup].indestructable = timerForPowerups(SDL_GetTicks(), 1500, currentPowerup);
        currentPowerup += 4; //Go to next place in array for next powerup being made
    }
     
     if(currentPowerup == POWERUPAMOUNT)
        currentPowerup = 0;
}

void renderPowerups(Game theGame)
{
    // //Draw powerups
    for(int i = 0; i < POWERUPAMOUNT; i++){
        if(theGame->powerups[i].isPickedUp == false) //Draw until picked up
        {
            SDL_Rect powerupRect = {theGame->powerups[i].x, theGame->powerups[i].y, 
                                    theGame->powerups[i].w, theGame->powerups[i].h};
        
            SDL_RenderCopy(theGame->renderer, theGame->texturePowerups[theGame->powerups[i].type], NULL, &powerupRect); 
        }
    }
}

void powerupGive(Game theGame, int playerID, int i)
{
    if(theGame->powerups[i].type == 0){ //give speed
        playerIncreaseSpeed(&theGame->player[playerID]);
    }
    else if(theGame->powerups[i].type == 1){ //give more power
        playerAddExplosionPower(&theGame->player[playerID], 1);
    }
    else if(theGame->powerups[i].type == 2){ //give more bombs
        playerAddAmountOfBombs(&theGame->player[playerID], 1);
    }
    theGame->powerups[i].isPickedUp = true;
    theGame->updateFlag = true;
}

Powerup powerupPlace(int xPos, int yPos, int type)
{
    Powerup p;
    int powSize = WIDTH / 23.8;
    int powOffset = WIDTH / 119;
    p.w = powSize;
    p.h = powSize;
    p.x = xPos + powOffset;
    p.y = yPos + powOffset;
    p.type = type;
    p.isPickedUp = false;
    p.sentViaUDP = 0;
    p.id = 0;
    p.indestructable=1;
    return p;
}

int timerForPowerups(int startTime, int timeAmount, int powerUpID)
{
    static int lastTime[POWERUPAMOUNT] = {0}, currentTime[POWERUPAMOUNT] = {0};
    if(startTime != 0)
    {
        lastTime[powerUpID] = startTime;
    }
    currentTime[powerUpID] = SDL_GetTicks();
    if (currentTime[powerUpID] > lastTime[powerUpID] + timeAmount)
    {
        //lastTime[powerUpID] = currentTime[powerUpID];
        return 0;               //returnar 1 om tiden är ute
    }
    return 1;
}

PUBLIC int PowerUpGetXPosition(Powerup p)
{
    return p.x;
}

PUBLIC int PowerUpGetYPosition(Powerup p)
{
    return p.y;
}

PUBLIC int PowerUpGetWidth(Powerup p)
{
    return p.w;
}

PUBLIC int PowerUpGetHeight(Powerup p)
{
    return p.h;
}

PUBLIC int PowerUpGetType(Powerup p)
{
    return p.type;
}

PUBLIC int PowerUpGetIsPickedUp(Powerup p)
{
    return p.isPickedUp;
}

PUBLIC int PowerUpGetSentViaUDP(Powerup p)
{
    return p.sentViaUDP;
}

PUBLIC int PowerUpGetID(Powerup p)
{
    return p.id;
}

PUBLIC int PowerUpGetIndestructable(Powerup p)
{
    return p.indestructable;
}

PUBLIC void PowerUpSetID(Powerup *p, int i)
{
    p->id = i;
}

PUBLIC void PowerUpSetIndestructable(Powerup *p, int i)
{
    p->indestructable = i;
}

PUBLIC void PowerUpSetIsPickedUp(Powerup *p, int i)
{
    p->isPickedUp = i;
}

PUBLIC void PowerUpSetSentViaUDP(Powerup *p, int i)
{
    p->sentViaUDP = i;
}
