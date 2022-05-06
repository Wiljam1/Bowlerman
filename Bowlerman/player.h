#ifndef player_h
#define player_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"

#define PUBLIC /* empty */
#define PRIVATE static

#define LEFT_X 70
#define RIGHT_X 1076
#define TOP_Y 140
#define BOTTOM_Y 870

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
int getPlayerHeight();
int getPlayerWidth();
int getPlayerSpeed(Player p);
int getPlayerID(Player p);
PUBLIC void getStartPos(Player *p);
//ändrar spelarens (clientmässiga) hastighet beroende på användarens tangentbordsintryckningar
// void determinePlayerVelocity(Player playerMoving);

//ändrar spelarens (clientmässiga) position
//void playerPositionUpdate(int x, int y);

//inits a player_rect with the x-and-y-position of player
void initPlayerRect(SDL_Rect *playerRect, Player player1); 

//void destroyPlayer();

#endif