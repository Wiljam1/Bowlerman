#ifndef powerup_h
#define powerup_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

#include "game.h"

typedef struct powerup Powerup;

void rollForPowerup(Game theGame, int xPos, int yPos); //Roll for a chance to place a random powerup
void renderPowerups(Game theGame);                     //Render powerups
Powerup initPowerups(int amount, int width, int height); //How many powerups are going to be used
Powerup powerupPlace(int xPos, int yPos, char type); //Place a powerup in the window

#endif