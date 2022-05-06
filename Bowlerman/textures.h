#ifndef textures_h
#define textures_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"

#define PUBLIC /* empty */
#define PRIVATE static

SDL_Texture *loadTextures(Game newGame, char fileLocation[]); // loadmedia
void loadAllTextures(Game theGame);
void renderTextures(Game theGame);


#endif