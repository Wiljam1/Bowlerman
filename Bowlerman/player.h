#ifndef player_h
#define player_h

#include <stdio.h>

typedef struct playerController *Player;
Player initPlayer(float xPos, float yPos);

float getPlayerXPosition(Player p);
float getPlayerYPosition(Player p);
int getPlayerHeight();
int getPlayerWidth();

//void destroyPlayer();

#endif