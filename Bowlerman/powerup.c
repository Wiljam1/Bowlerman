#include "powerup.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"
#include "game.h"

void rollForPowerup(Game theGame, int xPos, int yPos)
{
    static int currentPowerup = 0;            //Kanske vill göra på ett annat sätt här
    theGame->powerups[currentPowerup] = initPowerup(70, 70); //Which place in array of powerups

    printf("Powerup height: %d\n", theGame->powerups[currentPowerup].h);
    printf("Current Powerup: %d\n", currentPowerup);

    //if(1) // Implement random chance in future
    //{                                                      //Implement random chance for type in future
        theGame->powerups[currentPowerup] = powerupPlace(xPos, yPos, 's'); 
        printf("Powerup xPos: %d\n", theGame->powerups[currentPowerup].x);
        printf("Powerup yPos: %d\n", theGame->powerups[currentPowerup].y);
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

        //printf("Powerup xPos: %d\n", theGame->powerups[i].x);
        //printf("Powerup yPos: %d\n------------\n", theGame->powerups[i].y);

        //Add if statements to determine which texture to draw depending on type of powerup in future
        SDL_RenderCopy(theGame->renderer, theGame->texturePowerups[0], NULL, &powerupRect);
    }
}

Powerup initPowerup(int width, int height)
{
    Powerup p;

    //p.id = amount;
    p.w = width;
    p.h = height;

    return p;
}

Powerup powerupPlace(int xPos, int yPos, char type)
{
    Powerup p;

    p.x = xPos;
    p.y = yPos;
    p.type = type;
    
    return p;
}