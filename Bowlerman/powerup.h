#ifndef powerup_h
#define powerup_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

#include "game.h"

typedef struct powerup Powerup;

#define POWERUPTYPES 3

void rollForPowerup(Game theGame, int xPos, int yPos); //Rolls for a chance to place a powerup on the X/Y-pos given to the function.
void renderPowerups(Game theGame);                     //Render all the powerups.
void powerupGive(Game theGame, int playerID, int i);   //Grant powerup to player who picks it up.
Powerup powerupPlace(int xPos, int yPos, int type);    //Create and place a powerup on X/Y-pos given to the function.
int timerForPowerups(int startTime, int timeAmount, int powerUpID);
int PowerUpGetXPosition(Powerup p);
int PowerUpGetYPosition(Powerup p);
int PowerUpGetWidth(Powerup p);
int PowerUpGetHeight(Powerup p);
int PowerUpGetType(Powerup p);
int PowerUpGetIsPickedUp(Powerup p);
int PowerUpGetSentViaUDP(Powerup p);
int PowerUpGetID(Powerup p);
int PowerUpGetIndestructable(Powerup p);
void PowerUpSetID(Powerup *p, int i);
void PowerUpSetIndestructable(Powerup *p, int i);
void PowerUpSetIsPickedUp(Powerup *p, int i);
void PowerUpSetSentViaUDP(Powerup *p, int i);
char getMoveDirection(Player p);

#endif