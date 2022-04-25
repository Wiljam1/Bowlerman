#ifndef game_h
#define game_h
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "player.h"


struct game_type
{
    //player
    int playerAmount;    //how many players are online
    int playerID;        //the players ID. 
    Player player[4];
   

    //Rectangles
    SDL_Rect bombRect;

    SDL_Window  *window;
    SDL_Surface *window_surface;

    //Renderer
    SDL_Renderer *renderer;

    //Images
    SDL_Texture *background;
    SDL_Texture *player_texture[4];     //4  players, måste stå 4 annars blir de segmentation fault.
    SDL_Texture *bomb_texture;
    
    SDL_Event    window_event;
}; 

typedef struct game_type *Game;

Game initializeGameWindow();              // Struct for app initialization
SDL_Texture *loadTextures(Game newGame, char fileLocation[]); // Load any image you want in the resources/ folder!
void gameUpdate(Game newGame);  // Game loop
bool checkEvents(Game theGame);
void manageMovementInputs(Game theGame);
void updatePlayerPos(Game theGame, int playerID, int velX, int velY); //Flytta till player.c nångång
void renderTextures(Game theGame);
void destroyGame(Game theGame); // Function for easily destroying the application.

#endif