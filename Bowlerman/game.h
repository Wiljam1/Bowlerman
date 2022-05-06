#ifndef game_h
#define game_h
#include <stdbool.h>
#include <SDL2/SDL.h>

//denna .h-fil får INTE include:a några andra .h-filer. För då blir det "circular inclusion".
//pga alla andra .h-filer är beroende av game.h


//values to be set by menu in the future
//----------------------------------------------
#define PLAYERTEXTURES 4
#define MAXPLAYERS 4    //how many players are online

#define PLAYERAMOUNT 4    //how many players are online
#define WALLAMOUNT 100
#define POWERUPAMOUNT 100 //How many powerups per map  

#define WIDTH 1190 // Move eventually
#define HEIGHT 910 + 100
//-------------------------------------------------

struct wall{
    int id; //UDP?
    int destroyedWall;
    int x, y, w, h;
};
typedef struct wall Wall;

struct powerup{
    int id; //UDP?
    int x, y, w, h;
    char type; // 's' = speed, 'm' = moreBombs, 'x' = explosionPower, 'r' = canRollBombs...
};
typedef struct powerup Powerup;

struct bowling_ball
{
    //int id;   
    //float yVel, xVel, xPos, yPos;
    //float xPosOld;
    //float yPosOld;
    SDL_Rect position;
    //int height;
    //int width;
    int timervalue;
    int speed;
    int timerinit;
    int explosioninit;
    int placedBombRestriction;
    int powerUpExplosion;
    int isPlaced;

    //int frame;
};
struct bowling_ball typedef Bowlingball;

SDL_Rect bowlingballAnimation[18];

//när jag har denna struct i player.c funkar koden inte i game.c jättekonstigt! fattar ej varför
//jag tror det är för att programmet måste veta hur stor struct:en är (?)
struct playerController
{
    int id;   //för packets via UDP behöver man kunna veta vem det är som skickar datan.
    bool up, down, left, right;
    float yVel, xVel, xPos, yPos;
    float xPosOld;
    float yPosOld;
    bool isMoving; //is the player moving or not

    float speed;
    int amountOfBombs, explosionPower;
    bool canRollBombs;

    int height;
    int width;
    char name[40];
    bool isDead;
    SDL_Rect playerRect;   //struct to hold the position and size of the sprite
    char moveDirection;  // Keeps track of player movement direction for sprite rendering
};
typedef struct playerController Player;

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
    Player player[MAXPLAYERS];
    int playerIDLocal;        //the local players ID (only on this computer). 
    int playerAmount;  //amount of players online
    SDL_Rect playerRect[4][3];
    PlayerSprites pSprites;

    //Walls
    Wall wall[WALLAMOUNT*3];
    
    //Powerups
    Powerup powerups[POWERUPAMOUNT];

    //bombs
    Bowlingball bombs[PLAYERAMOUNT];
    SDL_Rect *bowlingballAnimation[18];
    SDL_Rect explosionPosition[PLAYERAMOUNT][WALLAMOUNT];
    int allowBombPlacement[PLAYERAMOUNT];

    //Renderer
    SDL_Renderer *renderer;

    //Images
    SDL_Texture *background;
    SDL_Texture *player_texture[MAXPLAYERS][PLAYERTEXTURES];     //4  players, måste stå 4 annars blir de segmentation fault.
    SDL_Texture *bomb_texture[PLAYERAMOUNT];
    SDL_Texture *textureWall[4];
    SDL_Texture *bombExplosion_texture;
    SDL_Texture *texturePowerups[4];

    SDL_Event    window_event;
};
typedef struct game_type *Game;
Game createWindow();              // Struct for app initialization
SDL_Texture *loadTextures(Game newGame, char fileLocation[]); // Load any image you want in the resources/ folder!
void gameUpdate(Game newGame);  // Game loop
bool checkEvents(Game theGame);
void manageMovementInputs(Game theGame);
void updatePlayerPos(Game theGame, int velX, int velY); //Flytta till player.c nångång
void process(Game theGame);
void collisionDetect(Game theGame);
void renderTextures(Game theGame);
void destroyGame(Game theGame); // Function for easily destroying the application.

#endif