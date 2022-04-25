#ifndef bomb_h
#define bomb_h
#include <SDL2/SDL.h>
#include "player.h"
#include "game.h"


typedef struct Bowling_ball *Bowlingball;

Bowlingball createBowlingBall(Player player);

int getBowlingBallPositionX(Bowlingball ball);
int getBowlingBallPositionY(Bowlingball ball);
int getBowlingBallWidth();
int getBowlingBallHeight();
int getBowlingBallFrame(Bowlingball ball);
void loadBomb(Game theGame);
//void loadBomb();




#endif