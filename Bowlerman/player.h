#ifndef player_h
#define player_h

#include <stdio.h>

typedef struct playerController *Player;
Player initPlayer();

float getPlayerXPos(Player p);
float getPlayerYPos(Player p);
int getPlayerHeight();
int getPlayerWidth();

//void destroyPlayer();

#endif