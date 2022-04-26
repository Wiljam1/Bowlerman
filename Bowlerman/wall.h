#ifndef wall_h
#define wall_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct wall Wall;

struct wall{
    int id; //UDP?

    int x, y, w, h;
};

Wall initWalls(int amount, int width, int height);  //How many walls are going to be used
Wall wallPlace(int x, int y);  //Place a wall in the window
void wallDelete(int id); //Delete a placed wall

#endif