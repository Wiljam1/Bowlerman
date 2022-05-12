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
    if((rand() % 100+1) < 33){ // 33% chance to spawn a powerup                                                      
        theGame->powerups[currentPowerup] = powerupPlace(xPos, yPos, rand()%POWERUPTYPES); //Type = random number between 0 and how many types there are
        theGame->powerups[currentPowerup].id = currentPowerup;
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
        playerAddSpeed(&theGame->player[playerID], 0.5);
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
    p.w = 50;
    p.h = 50;
    p.x = xPos + 10;
    p.y = yPos + 10;
    p.type = type;
    p.isPickedUp = false;
    p.sentViaUDP = 0;
    p.id = 0;
    return p;
}