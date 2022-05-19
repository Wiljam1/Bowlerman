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
void sortBombsArray(Game theGame,int i);
void initExplosionPosition(Game theGame, int playerID);
Bowlingball initBomb();
void loadBomb();
int timeleft(int time, int playerID);
int getBombXPosition(Bowlingball b);
int getBombYPosition(Bowlingball b);
int getBombHeight(Bowlingball b);
int getBombWidth(Bowlingball b);
int BombGetPlacedBombRestriction(Bowlingball b);
int BombGetIsPlaced(Bowlingball b);
int BombGetTimerValue(Bowlingball b);
int BombGetTimerInit(Bowlingball b);
int BombGetExplosionInit(Bowlingball b);

void BombSetIsPlaced(Bowlingball *b, int i);
void BombSetXPosition(Bowlingball *b, int i);
void BombSetYPosition(Bowlingball *b, int i);
void BombSetTimerValue(Bowlingball *b, int i);
void BombSetWhoPlacedID(Bowlingball *b, int i);
void BombSetStartvaluetimerbomb(Bowlingball *b, int i);
void BombSetExplosionInit(Bowlingball *b, int i);
void BombSetTimerInit(Bowlingball *b, int i);
void BombSetTimerValue(Bowlingball *b, int i);
int BombSetPlacedBombRestriction(Bowlingball b, int i);



#endif