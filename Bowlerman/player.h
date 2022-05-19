#ifndef player_h
#define player_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"

#define PUBLIC /* empty */
#define PRIVATE static


// #define PLAYERHEIGHT = 100;
// #define PLAYERWIDTH = 50;    //Använder structen för sånt här istället
// #define PLAYERSPEED = 5;     // speed in pixels/second

//nånting konstigt med player.h elr player.c, tror att de blir inkluderade två gånger
//Får en massa errors när man flyttar runt saker här "already defined blah blah blah"

typedef struct playerController Player;
typedef struct playerSprites PlayerSprites;

//initiate player position with x and y coordinates
PlayerSprites GetPlayerSprite();
Player initPlayer(int xPos, int yPos, int playerID);

void UpdatePlayerTextures(Game theGame);
float playerGetXPosition(Player p);
float playerGetYPosition(Player p);
int playerGetHeight(Player p);
int playerGetWidth(Player p);
double getPlayerSpeed(Player p);
int getPlayerID(Player p);
void getStartPos(Player *p);
int playerGetNoOfLives(Player p);
void playerIncreaseSpeed(Player *p);
void playerAddSpeedDisplay(Player *p, int speedDisplay);
void playerAddExplosionPower(Player *p, int explosionPower);
void playerAddAmountOfBombs(Player *p, int amountOfBombs);
void playerAddLives(Player *p, int amountOfBombs);
void playerAddScore(Player *p, int score);
//PUBLIC void getStartPos(Player *p);
PUBLIC int getLocalID(Game theGame);
int playerGetAmountOfBombsPlaced(Player p);
int playerGetAmountOfBombs(Player p);
int playerGetIsInvulnerable(Player p);
void playerAddAmountOfBombsPlaced(Player *p, int i);
int playerGetExplosionPower(Player p);
void playerSetID(Player *p, int id);

void playerSetXPos(Player *p, float x);
void playerSetYPos(Player *p, float y);

void playerSetOldXPos(Player *p, float x);
void playerSetOldYPos(Player *p, float y);
int playerGetOldXpos(Player p);
int playerGetOldYPos(Player p);
int playerGetScore(Player p);

void playerSetNoOfLives(Player *p, int lives);
void playerSetScore(Player *p, int score);
char playerGetMoveDirection(Player p);
//Change players movedirection
void playerSetMoveDirection(Player *p, char c);
//Change player x position
void updatePlayerXPosition(Player *p, double velX);
//Change player y position
void updatePlayerYPosition(Player *p, double velY);

//ändrar spelarens (clientmässiga) position
//void playerPositionUpdate(int x, int y);

//inits a player_rect with the x-and-y-position of player
void initAllPlayers(Game theGame);
void initPlayerRect(SDL_Rect *playerRect, Player player); 
void playerDeathTimer(Game theGame);
void setPlayerDeathFlags(Game theGame, int i);
//void destroyPlayer();

#endif