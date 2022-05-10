#ifndef gui_h
#define gui_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "game.h"


void initGUI(Game theGame);
void updateGUI(Game theGame);
void drawGUI(Game theGame);
void destroyGUI(Game theGame);

#endif