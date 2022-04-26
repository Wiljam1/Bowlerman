#ifndef bomb_h
#define bomb_h
#include <SDL2/SDL.h>
#include "player.h"
#include "game.h"
struct bowling_ball typedef Bowlingball;

struct bowling_ball
{
    //int id;   
    //float yVel, xVel, xPos, yPos;
    //float xPosOld;
    //float yPosOld;
    SDL_Rect possition;
    //int height;
    //int width;
    int speed;
    //int frame;
};

SDL_Rect bowlingballAnimation[18];

Bowlingball initBomb(int playerID);
void loadBomb();

//SDL_Rect setBowlingBallPosition(Game theGame, Player player);

//Bowlingball createBowlingBall();

//int getBowlingBallPositionX(Bowlingball ball);
//int getBowlingBallPositionY(Bowlingball ball);
//int getBowlingBallWidth();
//int getBowlingBallHeight();
//int getBowlingBallFrame(Bowlingball ball);





#endif