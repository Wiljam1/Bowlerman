#ifndef bomb_h
#define bomb_h
#include <SDL2/SDL.h>
#include "game.h"

typedef struct playerController *Player;
#define PLAYERAMOUNT 4

//void allowBombPlacementInit(Game theGame);
struct bowling_ball typedef Bowlingball;                                //define för structen
int initbowlingballtimer(int startTime, int timeAmount, int playerID);  //initierar bomb timer
int correctBowlingBallPosx(int i);                                      // korrigerar bombernas position så de blir centrerade
int correctBowlingBallPosy(int i);                                      //samma som ovan
Bowlingball initBomb();                                                 //skapar bomb
void sortBombsArray(Game theGame,int i, Player p[]);                                //sorterar bombernas array
void tryToPlaceBomb(Game theGame, int playerID, Player p[]);                        //testar att lägga ut bomb och gör det om det är tillåtet
void initExplosionPosition(Game theGame, int playerID, Player p[]);                 //initierar exposionspositionen
void loadBomb();                                                        
//int timeleft(int time, int playerID);                                   
int getBombXPosition(Bowlingball b);                                    //resterande är obvious vad de gör, läs funktionsnamnet
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
void BombSetPlacedBombRestriction(Bowlingball *b, int i);



#endif