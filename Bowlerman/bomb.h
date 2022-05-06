#ifndef bomb_h
#define bomb_h
#include <SDL2/SDL.h>
#include "player.h"
#include "game.h"

#define PLAYERAMOUNT 4


struct bowling_ball typedef Bowlingball;



int initbowlingballtimer(int startTime, int timeAmount, int playerID);

int correctBowlingBallPos(int i);

//int checkbowlingballtimer();



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