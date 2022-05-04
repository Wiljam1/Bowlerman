#include "wall.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"
#include "game.h"
#include "collissionDetection.h"

Wall initWalls(int amount, int width, int height)
{
    //Wall w = malloc(sizeof(struct wall)); //Fixa nångång när vi vet mer om minnesallokering
    Wall w;

    w.id = amount;
    w.w = width;
    w.h = height;
    w.destroyedWall = 0;

    return w;
}

Wall wallPlace(int xPos, int yPos)
{
    Wall w;

    w.x = xPos;
    w.y = yPos;

    return w;
}  //Place a wall

void wallDelete(int id)
{
    printf("wallDelete(); not implemented\n");
} //Delete a placed wall