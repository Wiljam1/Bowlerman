#ifndef wall_h
#define wall_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>


#include "game.h"

struct wall{
    int id; //UDP?
    int destroyedWall;
    float x, y; 
    int w, h;
};
typedef struct wall Wall;

Wall wall[WALLAMOUNT*3];

//typedef struct wall Wall; //Skrivet i game.h också? Kolla på i framtiden

void renderWalls(Game theGame);                     //Render walls.
void initAllWalls(Wall wall[]);                    //Create all wall objects used in the game.
Wall initWalls(int amount, int width, int height);  //How many walls are going to be used
Wall wallPlace(float xPos, float yPos);             //Place a wall in the window on X/Y-pos given in the function.
void wallDelete(int id);                            //Delete a placed wall (not implemented)
int getWallXPosition(Wall w);                       //Returns X position of wall.
int getWallYPosition(Wall w);                       //Returns Y position of wall.
int getWallHeight(Wall w);                          //Returns height of wall.
int getWallWidth(Wall w);                           //Returns width of wall.
int WallGetDestroyedWall(Wall w);
void WallSetDestroyedWall(Wall *w, int i);

#endif