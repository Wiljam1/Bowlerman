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

void initAllWalls(Game theGame)
{
      // Init walls / map
    float wallwidth = WIDTH/17; // Vet inte hur vi ska bestämma dehär variablerna riktigt,
    int wallheight = WIDTH/17;  // Om de ens kommer användas, väggarna kommer ju alltid vara den här storleken?
    for (int i = 0; i < WALLAMOUNT; i++)
    {
        theGame->wall[i] = initWalls(WALLAMOUNT, wallwidth, wallheight);
        if (i < 20)
        {
            theGame->wall[i] = wallPlace(i * wallwidth, 100);
        }
        else if (i < 40)
        {
            theGame->wall[i] = wallPlace(wallwidth * (i - 20), HEIGHT - wallheight);
        }
        else if (i < 60)
        {
            theGame->wall[i] = wallPlace(0, (i - 40) * wallheight + 100);
        }
        else if (i < 80)
        {
            theGame->wall[i] = wallPlace(WIDTH - wallwidth, (i - 60) * wallheight + 100);
        }
        else
        {
        }
    }
    //initiering av oförstörbara väggar i mitten av planen.
    int count=-1;
    for (int i=0;i<5;i++)
    {
        for(int j=0;j<7;j++)
        {
            count ++;
            theGame->wall[count+WALLAMOUNT] = initWalls(WALLAMOUNT*3, wallwidth, wallheight);
            theGame->wall[count+WALLAMOUNT] = wallPlace(j*wallwidth*2+wallwidth*2, i*wallwidth*2+wallwidth*2 + 100);
        }
    }
    //initiering av förstörbara väggar i planen. 
    count=39;
    for (int i=1;i<12;i++)
    {
        for(int j=0;j<9;j++)
        {
            count ++;
            theGame->wall[count+WALLAMOUNT] = initWalls(WALLAMOUNT*3, wallwidth, wallheight);
            theGame->wall[count+WALLAMOUNT] = wallPlace(j*wallwidth+wallwidth*4, i*wallwidth+100);
        }
        i+=9;
    }
    for (int i=2;i<12;i++)
    {
        for(int j=0;j<6;j++)
        {
            count ++;
            theGame->wall[count+WALLAMOUNT] = initWalls(WALLAMOUNT*3, wallwidth, wallheight);
            theGame->wall[count+WALLAMOUNT] = wallPlace(j*wallwidth*2+wallwidth*3, i*wallwidth+100);
        }
        i+=7;
    }
    for (int i=3;i<12;i++)
    {
        for(int j=0;j<13;j++)
        {
            count ++;
            theGame->wall[count+WALLAMOUNT] = initWalls(WALLAMOUNT*3, wallwidth, wallheight);
            theGame->wall[count+WALLAMOUNT] = wallPlace(j*wallwidth+wallwidth*2, i*wallwidth+100);
        }
        i+=5;
    }
    for (int i=4;i<9;i++)
    {
        for(int j=0;j<8;j++)
        {
            count ++;
            theGame->wall[count+WALLAMOUNT] = initWalls(WALLAMOUNT*3, wallwidth, wallheight);
            theGame->wall[count+WALLAMOUNT] = wallPlace(j*wallwidth*2+wallwidth, i*wallwidth+100);
        }
        i++;
    }
    for (int i=5;i<8;i++)
    {
        for(int j=0;j<15;j++)
        {
            count ++;
            theGame->wall[count+WALLAMOUNT] = initWalls(WALLAMOUNT*3, wallwidth, wallheight);
            theGame->wall[count+WALLAMOUNT] = wallPlace(j*wallwidth+wallwidth, i*wallwidth+100);
        }
        i++;
    }
}

void renderWalls(Game theGame)
{
    // Draw walls
    for (int i = 77; i >= 0; i--)
    {
        SDL_Rect wallRect = {theGame->wall[i].x, theGame->wall[i].y, theGame->wall[i].w, theGame->wall[i].h};
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
        SDL_Rect wallRect = {theGame->wall[i+WALLAMOUNT].x, theGame->wall[i+WALLAMOUNT].y, theGame->wall[i+WALLAMOUNT].w, theGame->wall[i+WALLAMOUNT].h};
        SDL_RenderCopy(theGame->renderer, theGame->textureWall[2], NULL, &wallRect);        
    }
    for (int i=139;i<250;i++)
    {   
        if (theGame->wall[i].destroyedWall == 0)
        {
            SDL_Rect wallRect = {theGame->wall[i].x, theGame->wall[i].y, theGame->wall[i].w, theGame->wall[i].h};
            SDL_RenderCopy(theGame->renderer, theGame->textureWall[3], NULL, &wallRect);   
        }     
    }
}