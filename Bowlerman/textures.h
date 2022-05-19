#ifndef textures_h
#define textures_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"

#define PUBLIC /* empty */
#define PRIVATE static

SDL_Texture *loadTextures(Game theGame, char fileLocation[]); //Load media from given file location
void loadAllTextures(Game theGame);                           //Load all textures
void renderTextures(Game theGame);                            //Render textures
void renderPowerups(Game theGame);                     //Render all the powerups.


#endif