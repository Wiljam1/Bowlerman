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

// typedef enum powerupEnum{SPEED, POWER, BOMBS, TURTLE} powerup;

// powerup randomPowerup()
// {
    
// }

void rollForPowerup(Game theGame, int xPos, int yPos)
{
    //enum powerupEnum powerup = randomPowerup();
    static int currentPowerup = 0;            //Kanske vill göra på ett annat sätt här

    //if(1) // Implement random chance in future
    //{                                                      //Type = random number between 0 and how many types there are
        theGame->powerups[currentPowerup] = powerupPlace(xPos, yPos, rand()%POWERUPTYPES); 
    //}
     currentPowerup++; //Go to next place in array for next powerup being made
     if(currentPowerup == POWERUPAMOUNT)
        currentPowerup = 0;
}

void renderPowerups(Game theGame)
{
    // //Draw powerups                  //This works but not when assigned new 
    for(int i = 0; i < POWERUPAMOUNT; i++){
        SDL_Rect powerupRect = {theGame->powerups[i].x, theGame->powerups[i].y, 
                                theGame->powerups[i].w, theGame->powerups[i].h};
        
        SDL_RenderCopy(theGame->renderer, theGame->texturePowerups[theGame->powerups[i].type], NULL, &powerupRect);
    }
}

Powerup powerupPlace(int xPos, int yPos, int type)
{
    Powerup p;
    p.w = 50;
    p.h = 50;
    p.x = xPos + 10;
    p.y = yPos + 10;
    p.type = type;
    
    return p;
}