#ifndef game_h
#define game_h
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "player.h"

#define PLAYERTEXTURES 13
#define MAXPLAYERS 4    //how many players are online

struct game_type
{
    SDL_Window  *window;
    SDL_Surface *window_surface;

    //Player
    Player player[MAXPLAYERS];

    int playerAmmount;

    //bombs
    SDL_Rect possition_ball;
    SDL_Rect *bowlingballAnimation[18];

    //Renderer
    SDL_Renderer *renderer;

    //Images
    SDL_Texture *background;
    SDL_Texture *player_texture[MAXPLAYERS][PLAYERTEXTURES];     //4  players, måste stå 4 annars blir de segmentation fault.
    SDL_Texture *bomb_texture;
    SDL_Texture *wall;

    SDL_Event    window_event;
}; 
typedef struct game_type *Game;
Game createWindow();              // Struct for app initialization
void initGame(Game theGame);
SDL_Texture *loadTextures(Game newGame, char fileLocation[]); // Load any image you want in the resources/ folder!
void gameUpdate(Game newGame);  // Game loop
bool checkEvents(Game theGame);
void manageMovementInputs(Game theGame);
void updatePlayerPos(Game theGame, int playerID, int velX, int velY); //Flytta till player.c nångång
void collisionDetect(Game theGame);
void renderTextures(Game theGame);
void destroyGame(Game theGame); // Function for easily destroying the application.

#endif