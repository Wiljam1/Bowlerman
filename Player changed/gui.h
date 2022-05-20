#ifndef gui_h
#define gui_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "game.h"
#include "player.h"


void initGUI(Game theGame);                                                               //Set font and init SDL_ttf
void createLabel(Game theGame, int labelID, char text[20], float value, SDL_Color color); //Create a label and place it in array
void updateGUI(Game theGame, Player p[]);                                                             //Update created labels
void drawGUI(Game theGame);
void destroyGUI(Game theGame);

#endif