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

    return w;
}

Wall wallPlace(int x, int y)
{
    Wall w;

    w.x = x;
    w.y = y;

    return w;
    //theGame->walls[id].x = x;
    //theGame->walls[id].y = y;
}  //Place a wall
void wallDelete(int id)
{
    printf("wallDelete(); not implemented\n");
} //Delete a placed wall