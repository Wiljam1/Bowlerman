#ifndef player_h
#define player_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

typedef struct playerController *Player;
Player initPlayer(float xPos, float yPos);

float getPlayerXPosition(Player p);
float getPlayerYPosition(Player p);
int getPlayerHeight();
int getPlayerWidth();


//rect for player
SDL_Rect playerRect[4]; 
//inits a player_rect with the x-and-y-position of player
void initPlayerRect(SDL_Rect *playerRect, Player player1); 



//void destroyPlayer();

#endif