#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "player.h"
#include "collissionDetection.h"
#include "sorter.h"
#include "bomb.h"





PUBLIC Bowlingball initBomb(int playerID)
{
    //malloc(sizeof(struct bowling_ball));
    Bowlingball b;
    b.position.y = 0;
    b.position.x = 0;
    b.position.w = 50;
    b.position.h = 50;
    b.speed = 0;
    b.timervalue[playerID] = 0;
    b.timerinit[playerID] = 0;
    b.explosioninit[playerID] = 1;
    return b;
}

int initbowlingballtimer(int startTime, int timeAmount)
{
    static int lastTime[4]={0}, currentTime[4] = {0};
    if(startTime != 0)
    {
        lastTime[0] = startTime;
    }
    currentTime[0] = SDL_GetTicks();
    if (currentTime[0] > lastTime[0] + timeAmount)
    {
        lastTime[0] = currentTime[0];
        return 1;
    }
    return 0;
}

/*
int checkbowlingballtimer(timervalue)
{
    int i=0, timeout=2000;
    if (SDL_TICKS_PASSED(timervalue, timeout))
    {
        i=1;
    }

    return i;
}
*/

PUBLIC void loadBomb()
{
    bowlingballAnimation[ 0 ].x =   0;
    bowlingballAnimation[ 0 ].y =   0;
    bowlingballAnimation[ 0 ].w = 256;
    bowlingballAnimation[ 0 ].h = 256;

    bowlingballAnimation[ 1 ].x = 256;
    bowlingballAnimation[ 1 ].y =   0;
    bowlingballAnimation[ 1 ].w = 256;
    bowlingballAnimation[ 1 ].h = 256;

    bowlingballAnimation[ 2 ].x = 512;
    bowlingballAnimation[ 2 ].y =   0;
    bowlingballAnimation[ 2 ].w = 256;
    bowlingballAnimation[ 2 ].h = 256;

    bowlingballAnimation[ 3 ].x = 768;
    bowlingballAnimation[ 3 ].y =   0;
    bowlingballAnimation[ 3 ].w = 256;
    bowlingballAnimation[ 3 ].h = 256;

    bowlingballAnimation[ 4 ].x =   0;
    bowlingballAnimation[ 4 ].y = 256;
    bowlingballAnimation[ 4 ].w = 256;
    bowlingballAnimation[ 4 ].h = 256;

    bowlingballAnimation[ 5 ].x = 256;
    bowlingballAnimation[ 5 ].y = 256;
    bowlingballAnimation[ 5 ].w = 256;
    bowlingballAnimation[ 5 ].h = 256;

    bowlingballAnimation[ 6 ].x = 512;
    bowlingballAnimation[ 6 ].y = 256;
    bowlingballAnimation[ 6 ].w = 256;
    bowlingballAnimation[ 6 ].h = 256;

    bowlingballAnimation[ 7 ].x = 768;
    bowlingballAnimation[ 7 ].y = 256;
    bowlingballAnimation[ 7 ].w = 256;
    bowlingballAnimation[ 7 ].h = 256;

    bowlingballAnimation[ 8 ].x =   0;
    bowlingballAnimation[ 8 ].y = 512;
    bowlingballAnimation[ 8 ].w = 256;
    bowlingballAnimation[ 8 ].h = 256;

    bowlingballAnimation[ 9 ].x = 256;
    bowlingballAnimation[ 9 ].y = 512;
    bowlingballAnimation[ 9 ].w = 256;
    bowlingballAnimation[ 9 ].h = 256;

    bowlingballAnimation[ 10 ].x = 512;
    bowlingballAnimation[ 10 ].y = 512;
    bowlingballAnimation[ 10 ].w = 256;
    bowlingballAnimation[ 10 ].h = 256;

    bowlingballAnimation[ 11 ].x = 768;
    bowlingballAnimation[ 11 ].y = 512;
    bowlingballAnimation[ 11 ].w = 256;
    bowlingballAnimation[ 11 ].h = 256;

    bowlingballAnimation[ 12 ].x =   0;
    bowlingballAnimation[ 12 ].y = 768;
    bowlingballAnimation[ 12 ].w = 256;
    bowlingballAnimation[ 12 ].h = 256;

    bowlingballAnimation[ 13 ].x = 256;
    bowlingballAnimation[ 13 ].y = 768;
    bowlingballAnimation[ 13 ].w = 256;
    bowlingballAnimation[ 13 ].h = 256;

    bowlingballAnimation[ 14 ].x = 512;
    bowlingballAnimation[ 14 ].y = 768;
    bowlingballAnimation[ 14 ].w = 256;
    bowlingballAnimation[ 14 ].h = 256;

    bowlingballAnimation[ 15 ].x = 768;
    bowlingballAnimation[ 15 ].y = 768;
    bowlingballAnimation[ 15 ].w = 256;
    bowlingballAnimation[ 15 ].h = 256;

    bowlingballAnimation[ 16 ].x =   0;
    bowlingballAnimation[ 16 ].y =1024;
    bowlingballAnimation[ 16 ].w = 256;
    bowlingballAnimation[ 16 ].h = 256;

    bowlingballAnimation[ 17 ].x = 256;
    bowlingballAnimation[ 17 ].y =1024;
    bowlingballAnimation[ 17 ].w = 256;
    bowlingballAnimation[ 17 ].h = 256;
}