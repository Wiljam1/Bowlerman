#ifndef game_h
#define game_h
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "player.h"

typedef struct game_type *Game;
Game createWindow();              // Struct for app initialization
void initGame(Game theGame);
SDL_Texture *loadTextures(Game newGame, char fileLocation[]); // Load any image you want in the resources/ folder!
void gameUpdate(Game newGame);  // Game loop
bool checkEvents(Game theGame);
void manageMovementInputs(Game theGame);
void updatePlayerPos(Game theGame, int playerID, int velX, int velY); //Flytta till player.c nångång
void renderTextures(Game theGame);
void destroyGame(Game theGame); // Function for easily destroying the application.

#endif