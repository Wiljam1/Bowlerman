#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "wall.h"
#include "player.h"
#include "game.h"
#include "collissionDetection.h"

struct wall{
    int id; //UDP?
    int destroyedWall;
    float x, y; 
    float w, h;
};

Wall initWalls(int amount, float width, float height)
{
    Wall w = malloc(sizeof(struct wall)); //Fixa nångång när vi vet mer om minnesallokering
    //Wall w;

    w->id = amount;
    w->w = width;
    w->h = height;
    w->destroyedWall = 0;
    w->x = 0;
    w->y = 0;

    return w;
}

void wallPlace(Wall w, float xPos, float yPos)
{
    w->x = xPos;
    w->y = yPos;
}  //Place a wall

void wallDelete(int id)
{
    printf("wallDelete(); not implemented\n");
} //Delete a placed wall

void initAllWalls(Wall *wall)
{
      // Init walls / map

    for (int i = 0; i < WALLAMOUNT*3; i++)  {
        wall[i] = initWalls(0, 0, 0);
    }
    
    float yOffset = WIDTH / 11.9;
    float wallwidth = WIDTH/17.0; // Vet inte hur vi ska bestämma dehär variablerna riktigt,
    float wallheight = WIDTH/17.0;  // Om de ens kommer användas, väggarna kommer ju alltid vara den här storleken?
    for (int i = 0; i < WALLAMOUNT; i++)
    {
        wall[i] = initWalls(WALLAMOUNT, wallwidth, wallheight);
        if (i < 20)
        {
            wallPlace(wall[i], i * wallwidth, yOffset);
        }
        else if (i < 40)
        {
            wallPlace(wall[i], wallwidth * (i - 20), HEIGHT - wallheight);
        }
        else if (i < 60)
        {
            wallPlace(wall[i], 0, (i - 40) * wallheight + yOffset);
        }
        else if (i < 80)
        {
            wallPlace(wall[i], WIDTH - wallwidth, (i - 60) * wallheight + yOffset);
        }
    }
    //initiering av oförstörbara väggar i mitten av planen.
    int count=-1;
    for (int i=0;i<5;i++)
    {
        for(int j=0;j<7;j++)
        {
            count ++;
            wall[count+WALLAMOUNT] = initWalls(WALLAMOUNT*3, wallwidth, wallheight);
            wallPlace(wall[count+WALLAMOUNT], j*wallwidth*2+wallwidth*2, i*wallwidth*2+wallwidth*2 + yOffset);
        }
    }
    //initiering av förstörbara väggar i planen. 
    count=39;
    for (int i=1;i<12;i++)
    {
        for(int j=0;j<9;j++)
        {
            count ++;
            wall[count+WALLAMOUNT] = initWalls(WALLAMOUNT*3, wallwidth, wallheight);
            wallPlace(wall[count+WALLAMOUNT], j*wallwidth+wallwidth*4, i*wallwidth+yOffset);
        }
        i+=9;
    }
    for (int i=2;i<12;i++)
    {
        for(int j=0;j<6;j++)
        {
            count ++;
            wall[count+WALLAMOUNT] = initWalls(WALLAMOUNT*3, wallwidth, wallheight);
            wallPlace(wall[count+WALLAMOUNT], j*wallwidth*2+wallwidth*3, i*wallwidth+yOffset);
        }
        i+=7;
    }
    for (int i=3;i<12;i++)
    {
        for(int j=0;j<13;j++)
        {
            count ++;
            wall[count+WALLAMOUNT] = initWalls(WALLAMOUNT*3, wallwidth, wallheight);
            wallPlace(wall[count+WALLAMOUNT], j*wallwidth+wallwidth*2, i*wallwidth+yOffset);
        }
        i+=5;
    }
    for (int i=4;i<9;i++)
    {
        for(int j=0;j<8;j++)
        {
            count ++;
            wall[count+WALLAMOUNT] = initWalls(WALLAMOUNT*3, wallwidth, wallheight);
            wallPlace(wall[count+WALLAMOUNT], j*wallwidth*2+wallwidth, i*wallwidth+yOffset);
        }
        i++;
    }
    for (int i=5;i<8;i++)
    {
        for(int j=0;j<15;j++)
        {
            count ++;
            wall[count+WALLAMOUNT] = initWalls(WALLAMOUNT*3, wallwidth, wallheight);
            wallPlace(wall[count+WALLAMOUNT], j*wallwidth+wallwidth, i*wallwidth+yOffset);
        }
        i++;
    }
}

void renderWalls(Game theGame, Wall *wall)
{
    // Draw walls
    for (int i = 77; i >= 0; i--)
    {
        SDL_Rect wallRect = {getWallXPosition(wall, i), getWallYPosition(wall, i), getWallWidth(wall, i), getWallHeight(wall, i)};
        /* LONG WALLS*/
        if (i < 36)
        {
            SDL_RenderCopy(theGame->renderer, theGame->textureWall[0], NULL, &wallRect);
        }
        else if (i < 76)
        {
            SDL_RenderCopyEx(theGame->renderer, theGame->textureWall[0], NULL, &wallRect, 90, 0, 0);
        }
        /* CORNER WALLS */
        if (i == 0)
        {
            SDL_RenderCopyEx(theGame->renderer, theGame->textureWall[1], NULL, &wallRect, 90, 0, 0);
        }
        if (i == 16)
        {
            SDL_RenderCopyEx(theGame->renderer, theGame->textureWall[1], NULL, &wallRect, 180, 0, 0);
        }
        if (i == 20)
        {
            SDL_RenderCopyEx(theGame->renderer, theGame->textureWall[1], NULL, &wallRect, 0, 0, 0);
        }
        if (i == 36)
        {
            SDL_RenderCopyEx(theGame->renderer, theGame->textureWall[1], NULL, &wallRect, 270, 0, 0);
        }
    }
    for (int i=0;i<36;i++)
    {   
        SDL_Rect wallRect = {getWallXPosition(wall, i+WALLAMOUNT), getWallYPosition(wall, i+WALLAMOUNT), getWallWidth(wall, i+WALLAMOUNT), getWallHeight(wall, i+WALLAMOUNT)};
        
        SDL_RenderCopy(theGame->renderer, theGame->textureWall[2], NULL, &wallRect);        
    }
    for (int i=139;i<250;i++)
    {   
        if (wall[i]->destroyedWall == 0)
        {
            SDL_Rect wallRect = {getWallXPosition(wall, i), getWallYPosition(wall, i), getWallWidth(wall, i), getWallHeight(wall, i)};
            SDL_RenderCopy(theGame->renderer, theGame->textureWall[3], NULL, &wallRect);   
        }     
    }
}

PUBLIC float getWallHeight(Wall *w, int id)
{
    return w[id]->h;
}
PUBLIC float getWallWidth(Wall *w, int id)
{
    return w[id]->w;
}
PUBLIC float getWallXPosition(Wall *w, int id)
{
    return w[id]->x;
}
PUBLIC float getWallYPosition(Wall *w, int id)
{
    return w[id]->y;
}

PUBLIC int WallGetDestroyedWall(Wall *w, int id)
{
    return w[id]->destroyedWall;
}

PUBLIC void WallSetDestroyedWall(Wall *w, int id, int i)
{
    w[id]->destroyedWall = i;
}