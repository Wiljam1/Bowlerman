#include "powerup.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include "player.h"

Powerup rollForPowerup(int *pCurrentPowerup, int ID, int xPos, int yPos)
{
    //printf("Rolling for powerup..\n");
    Powerup p;
    if((rand()%100+1) < 200){ // 40% chance to spawn a powerup                                                    
        p = powerupPlace(xPos, yPos, rand()%POWERUPTYPES); //Type = random number between 0 and how many types there are
        p.id = ID;
        p.sentViaUDP = false;
        p.indestructable = timerForPowerups(SDL_GetTicks(), 1500, ID);
        *pCurrentPowerup += 4; //Go to next place in array for next powerup being made
    }
    else{ //Create a powerup off-screen (to have something to put in the array and not cause crash)
        p = powerupPlace(3000+ID*10, 3000+ID*10, 0); //This powerup will eventually be overwritten by something else in the array
        p.id = ID;
        printf("created a powerup off-screen!\n");
    }
     if(*pCurrentPowerup == POWERUPAMOUNT){  //There will never be more than 100 powerups on the screen and the array starts over when reaching 100.
        *pCurrentPowerup = 0;
     }
    return p;
}

void powerupGive(Player pPlayer[], int id, Powerup *pPowerup)
{
    if(pPowerup->type == 0){ //give speed
        playerIncreaseSpeed(pPlayer, id);
    }
    else if(pPowerup->type == 1){ //give more power
        playerAddExplosionPower(pPlayer, id, 1);
    }
    else if(pPowerup->type == 2){ //give more bombs
        playerAddAmountOfBombs(pPlayer, id, 1);
    }
    PowerUpSetIsPickedUp(pPowerup, true);
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
    p.sentViaUDP = true;
    p.id = 0;
    p.indestructable=1;
    return p;
}

bool timerForPowerups(int startTime, int timeAmount, int powerUpID)
{
    static int lastTime[POWERUPAMOUNT] = {0}, currentTime[POWERUPAMOUNT] = {0};
    if(startTime != 0)
    {
        lastTime[powerUpID] = startTime;
    }
    currentTime[powerUpID] = SDL_GetTicks();
    if (currentTime[powerUpID] > lastTime[powerUpID] + timeAmount)
    {
        return false;               //returnar false om tiden är ute
    }
    return true;
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

PUBLIC void PowerUpSetIsPickedUp(Powerup *p, bool condition)
{
    p->isPickedUp = condition;
}

PUBLIC void PowerUpSetSentViaUDP(Powerup *p, int i)
{
    p->sentViaUDP = i;
}
