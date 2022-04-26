#ifndef game_h
#define game_h
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "player.h"
#include "bomb.h"

#define PLAYERTEXTURES 13
#define PLAYERAMOUNT 4    //how many players are online

struct game_type
{
    SDL_Window  *window;
    SDL_Surface *window_surface;

    //Player
    Player player[PLAYERAMOUNT];

    //bombs
    Bowlingball bombs[PLAYERAMOUNT];
    SDL_Rect *bowlingballAnimation[18];

    //Renderer
    SDL_Renderer *renderer;

    //Images
    SDL_Texture *background;
    SDL_Texture *player_texture[PLAYERAMOUNT][PLAYERTEXTURES];     //4  players
    SDL_Texture *bomb_texture[PLAYERAMOUNT];
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
void renderTextures(Game theGame);
void destroyGame(Game theGame); // Function for easily destroying the application.

#endif