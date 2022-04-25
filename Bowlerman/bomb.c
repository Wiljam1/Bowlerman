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

#define PUBLIC 
#define PRIVATE static

PRIVATE int BOWLINGBALL_WIDTH = 15;
PRIVATE int BOWLINGBALL_HEIGHT = 15;

PUBLIC SDL_Rect bowlingballAnimation[18];
PUBLIC SDL_Rect position_ball;

struct Bowling_ball
{
    int BOWLINGBALL_POSITION_X;
    int BOWLINGBALL_POSITION_Y;
    int BOWLINGBALL_HEIGHT;
    int BOWLINGBALL_WIDTH;
    int BOWLINGBALL_FRAME;
};


PUBLIC Bowlingball createBowlingBall(Player player)
{
    Bowlingball b = malloc(sizeof(struct Bowling_ball));
    b->BOWLINGBALL_POSITION_X = getPlayerXPosition(player);
    b->BOWLINGBALL_POSITION_Y = getPlayerYPosition(player);
    b->BOWLINGBALL_FRAME = 0;
    return b;
}


void loadBomb(Game theGame)
{
    //SDL_Surface* gbowlingballSurface = IMG_Load("resources/Bowling_Ball_BLue.png");
    //mbowlingball = SDL_CreateTextureFromSurface(gRenderer, gbowlingballSurface);


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

PUBLIC void setBowlingBallPosition(Player player, Bowlingball ball)
{

}

PUBLIC int getBowlingBallPositionX(Bowlingball ball){
    return ball->BOWLINGBALL_POSITION_X;
}


PUBLIC int getBowlingBallPositionY(Bowlingball ball){
    return ball->BOWLINGBALL_POSITION_Y;
}

PUBLIC int getBowlingBallWidth(){
    return BOWLINGBALL_WIDTH;
}

PUBLIC int getBowlingBallHeight(){
    return BOWLINGBALL_HEIGHT;
}

PUBLIC int getBowlingBallFrame(Bowlingball ball){
    return ball->BOWLINGBALL_FRAME;
}
