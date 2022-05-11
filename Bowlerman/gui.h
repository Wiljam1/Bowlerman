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
void createLabel(Game theGame, int labelID, char text[20], float value, SDL_Color color);
void updateGUI(Game theGame);
void drawGUI(Game theGame);
void destroyGUI(Game theGame);

#endif