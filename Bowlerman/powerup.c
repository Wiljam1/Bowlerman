#include "powerup.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"
#include "game.h"

enum week{Mon, Tue, Wed, Thur, Fri, Sat, Sun};

void rollForPowerup(Game theGame, int xPos, int yPos)
{
    static int currentPowerup = 0;            //Kanske vill göra på ett annat sätt här

    //if(1) // Implement random chance in future
    //{                                                      //Implement random chance for type in future
        theGame->powerups[currentPowerup] = powerupPlace(xPos, yPos, 's'); 
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

Powerup powerupPlace(int xPos, int yPos, char type)
{
    Powerup p;
    p.w = 50;
    p.h = 50;
    p.x = xPos + 10;
    p.y = yPos + 10;
    p.type = type;
    
    return p;
}