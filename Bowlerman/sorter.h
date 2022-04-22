#ifndef sorter_h
#define sorter_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"

//compares player[]->yPos 
//and returns a sorted array with the smallest number first
//the returned arary (v) is used for rendering players in the right order.
void arraySorter(Player *player, int length, int *v);

#endif