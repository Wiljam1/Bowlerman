#ifndef game_h
#define game_h
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
//#include "player.h"

//denna .h-fil får INTE include:a några andra .h-filer. För då blir det "circular inclusion".
//pga alla andra .h-filer är beroende av game.h


//values to be set by menu in the future
//----------------------------------------------
#define PLAYERTEXTURES 4
#define MAXPLAYERS 4    //how many players are online

#define PLAYERAMOUNT 4    //how many players are online
#define WALLAMOUNT 100
#define POWERUPAMOUNT 100 //How many powerups per map
#define TOP_ROW_LABELS 4
#define BOTTOM_ROW_LABELS 8
#define LABEL_AMOUNT (TOP_ROW_LABELS + BOTTOM_ROW_LABELS-4)

#define WIDTH 800.0 // Move eventually
#define HEIGHT (WIDTH / 1.178)
#define MAXBOMBAMOUNT 20
//-------------------------------------------------
#define LEN 20
struct sounds
{
    Mix_Music *backgroundMusic;
    Mix_Chunk *explosion;
    Mix_Chunk *death;
};
typedef struct sounds Sounds;

struct wall{
    int id; //UDP?
    int destroyedWall;
    float x, y; 
    float w, h;
};
typedef struct wall Wall;

struct powerup{
    int id; //UDP?
    int x, y, w, h;
    int type; // 0 = speed, 1 = power, 2 = morebombs, 3 = turtle
    bool isPickedUp;
    int sentViaUDP;
    int indestructable;
};
typedef struct powerup Powerup;

struct bowling_ball
{
    SDL_Rect position;
    int timervalue;
    int speed;
    int timerinit;
    int explosioninit;
    int placedBombRestriction;
    int isPlaced;
    int startvaluetimerbomb;
    int whoPlacedID;

    //int frame;
};
struct bowling_ball typedef Bowlingball;

SDL_Rect bowlingballAnimation[18];

/* struct playerController
{
    int id;   //för packets via UDP behöver man kunna veta vem det är som skickar datan.
    bool up, down, left, right;
    float yVel, xVel, xPos, yPos;
    float xPosOld;
    float yPosOld;
    bool isMoving; //is the player moving or not

    double speed;
    int amountOfBombs, explosionPower, amountOfBombsPlaced, speedDisplay;
    bool canRollBombs;

    int height;
    int width;
    char name[40];
    bool isDead, isInvulnerable;
    SDL_Rect playerRect;   //struct to hold the position and size of the sprite
    char moveDirection;  // Keeps track of player movement direction for sprite rendering
    int noOfLives, score;
}; */

struct playerSprites
{
    SDL_Rect BowlerManVert[8];
    SDL_Rect BowlerManHori[8];
};
typedef struct playerSprites PlayerSprites;


struct game_type
{
    SDL_Window  *window;
    SDL_Surface *window_surface;

    //Player
    //Player player[MAXPLAYERS];
    int playerIDLocal;        //the local players ID (only on this computer). 
    int playerAmount;  //amount of players online
    bool invulnerabiltyFlag[4];
    SDL_Rect playerRect[4][3];
    PlayerSprites pSprites;

    //Walls
    Wall wall[WALLAMOUNT*3];
    
    //Powerups
    Powerup powerups[POWERUPAMOUNT];
    int powerupsNotSent;

    //bombs
    Bowlingball bombs[MAXBOMBAMOUNT];
    SDL_Rect *bowlingballAnimation[18];
    SDL_Rect explosionPosition[MAXBOMBAMOUNT][WALLAMOUNT];

    //Renderer
    SDL_Renderer *renderer;

    //Hur snabbt spelet ska uppdateras. i millisekunder
    int delayInMS;

    //Images
    SDL_Texture *background;
    SDL_Texture *player_texture[MAXPLAYERS][PLAYERTEXTURES];     //4  players, måste stå 4 annars blir de segmentation fault.
    SDL_Texture *bomb_texture[PLAYERAMOUNT];
    SDL_Texture *textureWall[4];
    SDL_Texture *bombExplosion_texture;
    SDL_Texture *texturePowerups[4];
    SDL_Texture *menuBackground;
    SDL_Texture *logo;
    SDL_Texture *menuButtons[7];
    SDL_Texture *labels[LABEL_AMOUNT+1];                                        
    int labelW[LABEL_AMOUNT+1], labelH[LABEL_AMOUNT+1];
    bool updateFlag;

    //Fonts
    TTF_Font *font; //Gör till en array om vi ska ha mer fonts

    SDL_Event    window_event;
};
typedef struct game_type *Game;

Game createWindow();              // Struct for app initialization
SDL_Texture *loadTextures(Game theGame, char fileLocation[]); // Load any image you want in the resources/ folder!
void gameUpdate(Game theGame);  // Game loop
//void manageMovementInputs(Game theGame);
void updatePlayerPos(Game theGame, int velX, int velY); //Flytta till player.c nångång
void destroyGame(Game theGame); // Function for easily destroying the application.
void updateScoreFlag(Game theGame, bool cond);
void setLocalID(Game theGame, int id);
#endif