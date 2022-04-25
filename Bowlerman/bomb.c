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
PUBLIC SDL_Rect possition_ball;

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
