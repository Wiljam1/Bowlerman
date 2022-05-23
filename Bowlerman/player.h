#ifndef player_h
#define player_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"

#define PUBLIC /* empty */
#define PRIVATE static

#define LEFT_X (WIDTH/17)
#define RIGHT_X (WIDTH/1.106)
#define TOP_Y (WIDTH/8.5)
#define BOTTOM_Y (WIDTH/1.368)


// #define PLAYERHEIGHT = 100;
// #define PLAYERWIDTH = 50;    //Använder structen för sånt här istället
// #define PLAYERSPEED = 5;     // speed in pixels/second

typedef struct playerController *Player;

typedef struct playerSprites PlayerSprites;

//initiate Player p[]osition with x and y coordinates
PlayerSprites GetPlayerSprite();
//Player initPlayer(int xPos, int yPos, int playerID);
Player initPlayer(int initX, int initY, int playerID);
//Player playerGetLocalPlayer(Player p[]);

void UpdatePlayerTextures(Game theGame, Player p[]);
float playerGetXPosition(Player p[], int id);
float playerGetYPosition(Player p[], int id);
int playerGetHeight(Player p[], int id); //
int playerGetWidth(Player p[], int id); //
double getPlayerSpeed(Player p[], int id); //
int getPlayerID(Player p[], int id);
void getStartPos(Player p[], int id);
int playerGetNoOfLives(Player p[], int id);
void playerIncreaseSpeed(Player p[], int id); //
void playerAddSpeedDisplay(Player p[], int speedDisplay, int id); //
void playerAddExplosionPower(Player p[], int id, int explosionPower); //
void playerAddAmountOfBombs(Player p[], int id, int amountOfBombs); //
void playerAddLives(Player p[], int id, int amountOfBombs); //
void playerAddScore(Player p[], int id, int score); //
//PUBLIC void getStartPos(Player p[]);
PUBLIC int getLocalID(Game theGame);
int playerGetAmountOfBombsPlaced(Player p[], int id);
int playerGetAmountOfBombs(Player p[], int id);
int playerGetIsInvulnerable(Player p[], int id);
int playerGetIsDead(Player p[], int id);
void playerAddAmountOfBombsPlaced(Player p[], int id, int i); //
int playerGetExplosionPower(Player p[], int id);
void playerSetID(Player p[], int id);

void playerSetXPos(Player p[], int id, float x);
void playerSetYPos(Player p[], int id, float y);

void playerSetOldXPos(Player p[], int id, float x);
void playerSetOldYPos(Player p[], int id, float y);
int playerGetOldXpos(Player p[], int id);
int playerGetOldYPos(Player p[], int id);
int playerGetScore(Player p[], int id);

void playerSetDead(Player p[], int id);
void playerSetAlive(Player p[], int id);

void playerSetNoOfLives(Player p[], int id, int lives);
void playerSetScore(Player p[], int id, int score);
char playerGetMoveDirection(Player p[], int id);
//Change players movedirection
void playerSetMoveDirection(Player p[], int id, char c);
//Change player x position
void updatePlayerXPosition(Player p[], int id, double velX);
//Change player y position
void updatePlayerYPosition(Player p[], int id, double velY);

void playerSetCountForTimer(Player *p, int n, int id);

//ändrar spelarens (clientmässiga) position
//void playerPositionUpdate(int x, int y);

//inits a player_rect with the x-and-y-position of player
void initAllPlayers(Game theGame, Player p[]);
void initPlayerRect(SDL_Rect *playerRect, Player player); 
void playerDeathTimer(Game theGame, Player p[]);
void setPlayerDeathFlags(Game theGame, Player p[], int i);
void playerSetInvulnerability(Player p[], int id, bool b);
//void destroyPlayer();

int playerGetSpeedDisplay(Player p[], int id);

Player initAPlayer(int x, int y, int id);
void manageMovementInputs(Game theGame, Player p[]);

#endif