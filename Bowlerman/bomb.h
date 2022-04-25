#ifndef bomb_h
#define bomb_h
#include <SDL2/SDL.h>
#include "player.h"
#include "game.h"


typedef struct Bowling_ball *Bowlingball;

SDL_Rect bowlingballAnimation[18];
Bowlingball createBowlingBall(Player player);

int getBowlingBallPositionX(Bowlingball ball);
int getBowlingBallPositionY(Bowlingball ball);
int getBowlingBallWidth();
int getBowlingBallHeight();
int getBowlingBallFrame(Bowlingball ball);
//void loadBomb();




#endif