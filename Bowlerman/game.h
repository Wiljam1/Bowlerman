#ifndef game_h
#define game_h
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "player.h"
#include "bomb.h"
#include "wall.h"

//values to be set by menu in the future
#define PLAYERTEXTURES 4
#define MAXPLAYERS 4    //how many players are online


#define PLAYERAMOUNT 4    //how many players are online
#define WALLAMOUNT 100  

#define WIDTH 1190 // Move eventually
#define HEIGHT 910 + 100

struct game_type
{
    SDL_Window  *window;
    SDL_Surface *window_surface;

    //Player
    Player player[MAXPLAYERS];
    int playerIDLocal;        //the local players ID (only on this computer). 
    int playerAmount;  //amount of players online
    SDL_Rect playerRect[4][3];
    PlayerSprites pSprites;

    //Walls
    Wall wall[WALLAMOUNT*2];

    //bombs
    Bowlingball bombs[PLAYERAMOUNT];
    SDL_Rect *bowlingballAnimation[18];
    SDL_Rect explosionPosition[PLAYERAMOUNT][10];
    int allowBombPlacement[PLAYERAMOUNT];

    //Renderer
    SDL_Renderer *renderer;

    //Images
    SDL_Texture *background;
    SDL_Texture *player_texture[MAXPLAYERS][PLAYERTEXTURES];     //4  players, måste stå 4 annars blir de segmentation fault.
    SDL_Texture *bomb_texture[PLAYERAMOUNT];
    SDL_Texture *textureWall[3];
    SDL_Texture *bombExplosion_texture;

    SDL_Event    window_event;
};
typedef struct game_type *Game;
Game createWindow();              // Struct for app initialization
void initGame(Game theGame);
SDL_Texture *loadTextures(Game newGame, char fileLocation[]); // Load any image you want in the resources/ folder!
void gameUpdate(Game newGame);  // Game loop
bool checkEvents(Game theGame);
void manageMovementInputs(Game theGame);
void updatePlayerPos(Game theGame, int velX, int velY); //Flytta till player.c nångång
void collisionDetect(Game theGame);
void renderTextures(Game theGame);
void destroyGame(Game theGame); // Function for easily destroying the application.

#endif