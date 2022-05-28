#ifndef bomb_h
#define bomb_h
#include <SDL2/SDL.h>
#include "game.h"

typedef struct playerController *Player;
#define PLAYERAMOUNT 4

//define for struct
struct bowling_ball typedef Bowlingball;                
//the bomb timer
int initbowlingballtimer(int startTime, int timeAmount, int playerID);  
//fixes the bombs x position so it is centered to a tile
int correctBowlingBallPosx(int i);   
//fixes the bombs y position so it is centered to a tile                                  
int correctBowlingBallPosy(int i);    
//creates a bomb                                  
Bowlingball initBomb();         
//sorts the bomb array                                        
void sortBombsArray(Game theGame,int i, Player p[]);     
//tries to place a bomb and if possible it places a bomb
void tryToPlaceBomb(Game theGame, int playerID, Player p[]);                        
//SDL_Rect for explosion position
void initExplosionPosition(Game theGame, int playerID, Player p[]);  
//bomb rect for the sprite sheet               
void loadBomb();                                                        

//gets a bombs x position                                  
int getBombXPosition(Bowlingball b);    
//gets a bombs y position   
int getBombYPosition(Bowlingball b);
//gets a bombs height  
int getBombHeight(Bowlingball b);
//gets a bombs width  
int getBombWidth(Bowlingball b); 
//gets int for if a bomb is placed and a player is standing on it
int BombGetPlacedBombRestriction(Bowlingball b);
//gets value for if a bomb is placed
int BombGetIsPlaced(Bowlingball b);
//gets the timer value, if the timer has expired or not
int BombGetTimerValue(Bowlingball b);
//gets the value for if the bomb is placed and the timer is initiated
int BombGetTimerInit(Bowlingball b);
//gets the value for if a bombs explosion is initiated
int BombGetExplosionInit(Bowlingball b);

//sets the value for bomb isplaced
void BombSetIsPlaced(Bowlingball *b, int i);
//sets x position for bomb
void BombSetXPosition(Bowlingball *b, int i);
//sets y position for bomb
void BombSetYPosition(Bowlingball *b, int i);
//sets the timervalue for bomb if the timer is expired or not
void BombSetTimerValue(Bowlingball *b, int i);
//sets ID for who placed the bomb
void BombSetWhoPlacedID(Bowlingball *b, int i);
//sets the start value for the timer
void BombSetStartvaluetimerbomb(Bowlingball *b, int i);
//sets the explosion init value if explosion is initiated
void BombSetExplosionInit(Bowlingball *b, int i);
//sets the timer init value, if the bomb is placed and the timer is initiated
void BombSetTimerInit(Bowlingball *b, int i);
//sets the value for if a bomb is placed and a player is standing on it
void BombSetPlacedBombRestriction(Bowlingball *b, int i);



#endif