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

typedef struct playerController *Player;
typedef struct playerSprites PlayerSprites;

/*Gets the positions of the images in the sprites*/
PlayerSprites GetPlayerSprite();
/*Set start values for a player*/
Player initPlayer(int initX, int initY, int playerID);
/*Initializes max player amount*/
void initAllPlayers(Game theGame, Player p[]);
/*Update a players images depending on movement direction, status etc*/
void UpdatePlayerTextures(Game theGame, Player p[]);
/*Get a players x-pos*/
float playerGetXPosition(Player p[], int id);
/*Get a players y-pos*/
float playerGetYPosition(Player p[], int id);
/*Set a players x-pos*/
void playerSetXPos(Player p[], int id, float x);
/*Set a players y-pos*/
void playerSetYPos(Player p[], int id, float y);
/*Set a players old x-pos*/
void playerSetOldXPos(Player p[], int id, float x);
/*Set a players old y-pos*/
void playerSetOldYPos(Player p[], int id, float y);
/*Get a players old x-pos*/
int playerGetOldXpos(Player p[], int id);
/*Get a players old y-ps*/
int playerGetOldYPos(Player p[], int id);
/*Get a players height*/
int playerGetHeight(Player p[], int id);
/*Get a players width*/
int playerGetWidth(Player p[], int id);
/*Get a players speed*/
double getPlayerSpeed(Player p[], int id);
/*Get a players ID*/
int getPlayerID(Player p[], int id);
/*Set a players ID*/
void playerSetID(Player p[], int id);
/*Get remaining lives of a player*/
int playerGetNoOfLives(Player p[], int id);
/*Increases the speed of a player*/
void playerIncreaseSpeed(Player p[], int id);
/*Add a players speed to GUI*/
void playerAddSpeedDisplay(Player p[], int speedDisplay, int id);
/*Get the speed display number of a player*/
int playerGetSpeedDisplay(Player p[], int id);
/*Add explosion power to a player*/
void playerAddExplosionPower(Player p[], int id, int explosionPower);
/*Get the players explosion power*/
int playerGetExplosionPower(Player p[], int id);
/*Add bumber of bombs to a player*/
void playerAddAmountOfBombs(Player p[], int id, int amountOfBombs);
/*Add a life to a player*/
void playerAddLives(Player p[], int id, int lives);
/*Add score to a player*/
void playerAddScore(Player p[], int id, int score);
/*Get how many bombs a player has placed*/
int playerGetAmountOfBombsPlaced(Player p[], int id);
/*Get how many bombs a playet can place*/
int playerGetAmountOfBombs(Player p[], int id);
/*Get if a player is invulnerable*/
int playerGetIsInvulnerable(Player p[], int id);
/*Sets a players invulnerability, true or false*/
void playerSetInvulnerability(Player p[], int id, bool b);
/*Get if a player is dead*/
int playerGetIsDead(Player p[], int id);
/*Add how many bombs a player has placed*/
void playerAddAmountOfBombsPlaced(Player p[], int id, int i); //
/*Get a players score*/
int playerGetScore(Player p[], int id);
/*Set a player to dead*/
void playerSetDead(Player p[], int id);
/*Set a player to alive*/
void playerSetAlive(Player p[], int id);
/*Set number of lives for a player*/
void playerSetNoOfLives(Player p[], int id, int lives);
/*Set score for a player*/
void playerSetScore(Player p[], int id, int score);
/*Get movement direction for a player*/
char playerGetMoveDirection(Player p[], int id);
/*Change a players movedirection*/
void playerSetMoveDirection(Player p[], int id, char c);
/*Change a player x position*/
void updatePlayerXPosition(Player p[], int id, double velX);
/*Change a player y position*/
void updatePlayerYPosition(Player p[], int id, double velY);
/*Set amount of players from the player amount variable*/
void playerSetPlayerCount(Player *p, int n, int id);
/*Get amount of players from the player amount variable*/
int playerGetPlayerCount(Player *p);
/*Start the timer after a player has died*/
void playerDeathTimer(Game theGame, Player p[]);
/*Set neccesary flags after a players death*/
void setPlayerDeathFlags(Game theGame, Player p[], int i);
/*Manages movements of a player*/
void manageMovementInputs(Game theGame, Player p[]);

/* // Vet inte om denna används
void initPlayerRect(SDL_Rect *playerRect, Player player); */ 

#endif