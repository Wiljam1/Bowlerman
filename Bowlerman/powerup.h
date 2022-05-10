#ifndef powerup_h
#define powerup_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

#include "game.h"

typedef struct powerup Powerup;

#define POWERUPTYPES 3

void rollForPowerup(Game theGame, int xPos, int yPos); //Roll for a chance to place a random powerup
void renderPowerups(Game theGame);                     //Render powerups
void powerupGive(Game theGame, int playerID, int i);
Powerup powerupPlace(int xPos, int yPos, int type); //Place a powerup in the window

#endif