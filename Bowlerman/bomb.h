#ifndef bomb_h
#define bomb_h
#include <SDL2/SDL.h>
#include "player.h"
#include "game.h"

#define PLAYERAMOUNT 4

void allowBombPlacementInit(Game theGame);
struct bowling_ball typedef Bowlingball;

void tryToPlaceBomb(Game theGame, int playerID);

int initbowlingballtimer(int startTime, int timeAmount, int playerID);

int correctBowlingBallPos(int i);

//int checkbowlingballtimer();

void process(Game theGame);

void initExplosionPosition(Game theGame, int playerID);

Bowlingball initBomb();
void loadBomb();

//SDL_Rect setBowlingBallPosition(Game theGame, Player player);

//Bowlingball createBowlingBall();

//int getBowlingBallPositionX(Bowlingball ball);
//int getBowlingBallPositionY(Bowlingball ball);
//int getBowlingBallWidth();
//int getBowlingBallHeight();
//int getBowlingBallFrame(Bowlingball ball);





#endif