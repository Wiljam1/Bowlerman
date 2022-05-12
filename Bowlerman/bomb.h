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
int correctBowlingBallPosx(int i);
int correctBowlingBallPosy(int i);
void initExplosionPosition(Game theGame, int playerID);
Bowlingball initBomb();
void loadBomb();
int timeleft(int time, int playerID);
int getBombXPosition(Bowlingball b);
int getBombYPosition(Bowlingball b);
int getBombHeight(Bowlingball b);
int getBombWidth(Bowlingball b);



#endif