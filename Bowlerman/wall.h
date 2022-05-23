#ifndef wall_h
#define wall_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>


#include "game.h"

typedef struct wall *Wall; //Skrivet i game.h också? Kolla på i framtiden

void renderWalls(Game theGame, Wall *wall);                     //Render walls.
void initAllWalls(Wall *wall);                    //Create all wall objects used in the game.
Wall initWalls(int amount, float width, float height);  //How many walls are going to be used
void wallPlace(Wall w, float xPos, float yPos);             //Place a wall in the window on X/Y-pos given in the function.
void wallDelete(int id);                            //Delete a placed wall (not implemented)
float getWallXPosition(Wall *w, int id);                       //Returns X position of wall.
float getWallYPosition(Wall *w, int id);                       //Returns Y position of wall.
float getWallHeight(Wall *w, int id);                          //Returns height of wall.
float getWallWidth(Wall *w, int id);                           //Returns width of wall.
int WallGetDestroyedWall(Wall *w, int id);
void WallSetDestroyedWall(Wall *w, int id, int i);

#endif