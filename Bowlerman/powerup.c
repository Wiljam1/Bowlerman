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
    static int currentPowerups = 0;
    currentPowerups++;
    theGame->powerups[currentPowerups] = initPowerups(1, 70, 70);

    printf("Powerup height: %d\n", theGame->powerups[currentPowerups].h);

    //if(1) // Implement random chance in future
    //{                                                      //Implement random chance for type in future
        theGame->powerups[currentPowerups] = powerupPlace(xPos, yPos, 's'); 
        printf("Powerup xPos: %d\n", theGame->powerups[currentPowerups].x);
        printf("Powerup type: %c\n", theGame->powerups[currentPowerups].type);
    //}
}

void renderPowerups(Game theGame)
{
    //Draw powerups
    SDL_Rect testRect = {theGame->powerups[0].x, theGame->powerups[0].y, 
                        theGame->powerups[0].w, theGame->powerups[0].h};
    SDL_RenderCopy(theGame->renderer, theGame->texturePowerups[0], NULL, &testRect);
    
    for(int i = 0; i < 5; i++){
        SDL_Rect powerupRect = {theGame->powerups[i].x, theGame->powerups[i].y, 
                                theGame->powerups[i].w, theGame->powerups[i].h};
        //printf("Powerup xPos: %d\n", theGame->powerups[i].x);
        //printf("Powerup yPos: %d\n", theGame->powerups[i].y);
        
        //Add if statements to determine which texture to draw depending on type of powerup in future
        SDL_RenderCopy(theGame->renderer, theGame->texturePowerups[0], NULL, &powerupRect);
    }
}

Powerup initPowerups(int amount, int width, int height)
{
    Powerup p;

    p.id = amount;
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