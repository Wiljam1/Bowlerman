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
float getPlayerXPosition(Player p);
float getPlayerYPosition(Player p);
int getPlayerHeight(Player p);
int getPlayerWidth(Player p);
double getPlayerSpeed(Player p);
int getPlayerID(Player p);
void getStartPos(Player *p);
int getPlayerNoOfLives(Player p);
void playerIncreaseSpeed(Player *p);
void playerAddSpeedDisplay(Player *p, int speedDisplay);
void playerAddExplosionPower(Player *p, int explosionPower);
void playerAddAmountOfBombs(Player *p, int amountOfBombs);
void playerAddLives(Player *p, int amountOfBombs);
void playerAddScore(Player *p, int score);
//PUBLIC void getStartPos(Player *p);
PUBLIC int getLocalID(Game theGame);

void playerSetOldXPos(Player *p, int x);
void playerSetOldYPos(Player *p, int y);
int getPlayerOldXPos(Player p);
int getPlayerOldYPos(Player p);


//Change players movedirection
PUBLIC void updateMovementDirection(Game theGame, int id, char c);

//Change player x position
PUBLIC void updatePlayerXPosition(Game theGame, int id, double velX);
//Change player y position
PUBLIC void updatePlayerYPosition(Game theGame, int id, double velY);

//ändrar spelarens (clientmässiga) position
//void playerPositionUpdate(int x, int y);

//inits a player_rect with the x-and-y-position of player
void initAllPlayers(Game theGame);
void initPlayerRect(SDL_Rect *playerRect, Player player); 
void playerDeathTimer(Game theGame);
void setPlayerDeathFlags(Game theGame, int i);
//void destroyPlayer();

#endif