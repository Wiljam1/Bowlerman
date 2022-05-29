#ifndef game_h
#define game_h
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>


//denna .h-fil får INTE include:a några andra .h-filer. För då blir det "circular inclusion".
//pga alla andra .h-filer är beroende av game.h

#define PUBLIC /* empty */
#define PRIVATE static

//values to be set by menu in the future
//----------------------------------------------
#define PLAYERTEXTURES 4
#define MAXPLAYERS 4    //how many players are online

#define PLAYERAMOUNT 4    //how many players are online
#define WALLAMOUNT 100
#define POWERUPAMOUNT 100   //How many powerups can be spawned at once
#define TOP_ROW_LABELS 4    //Amount of labels in the top row of the GUI.
#define BOTTOM_ROW_LABELS 8 
#define LABEL_AMOUNT (TOP_ROW_LABELS + BOTTOM_ROW_LABELS - TOP_ROW_LABELS)

#define WIDTH 800.0 // Move eventually
#define HEIGHT (WIDTH / 1.178)        
#define MAXBOMBAMOUNT 20
//-------------------------------------------------
#define LEN 20

typedef struct sounds *Sounds;
typedef struct playerController *Player;

//Struct for object: Walls
struct wall{
    int id; 
    bool destroyedWall;
    float x, y, w, h;      //Rectangle-values for logic and rendering purposes.
};
typedef struct wall Wall;

//Struct for object: Powerups
struct powerup{
    int id;               //For keeping track of which powerup that is sent via UDP.
    int x, y, w, h;      
    int type;             // 0 = speed, 1 = power, 2 = more bombs
    bool isPickedUp;
    bool sentViaUDP;
    bool indestructable;  //Not able to be destroyed by bombs
};
typedef struct powerup Powerup;

//Struct for object: Bombs
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
};
struct bowling_ball typedef Bowlingball;

SDL_Rect bowlingballAnimation[18];

struct playerSprites
{
    SDL_Rect BowlerManVert[8];
    SDL_Rect BowlerManHori[8];
};
typedef struct playerSprites PlayerSprites;

// Master-struct for most data in the game (not a good practice)
struct game_type
{
    SDL_Window  *window;
    SDL_Surface *window_surface;

    //Player
    int playerIDLocal;          //the local players ID (only on this client). 
    int playerAmount;           //amount of players online
    bool invulnerabiltyFlag[4]; 
    SDL_Rect playerRect[4][3];  // *förklaring till varför det är en matris*
    PlayerSprites pSprites;

    //Walls
    Wall wall[WALLAMOUNT*3];    //How many walls that can be used at once. Stored in the array.
    
    //Powerups
    Powerup powerups[POWERUPAMOUNT];
    bool powerupsNotSent;       //A type of semaphore for the amount of powerups to send to other clients.

    //bombs
    Bowlingball bombs[MAXBOMBAMOUNT];
    SDL_Rect *bowlingballAnimation[18];
    SDL_Rect explosionPosition[MAXBOMBAMOUNT][WALLAMOUNT];

    //Renderer
    SDL_Renderer *renderer;

    //Delay between each game-loop iteration.
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
    TTF_Font *font;

    SDL_Event    window_event;
};
typedef struct game_type *Game;

Game createWindow();                                          // Struct for app initialization
SDL_Texture *loadTextures(Game theGame, char fileLocation[]); // Load any image from the resources/ folder!
void gameUpdate(Game theGame);                                // Game-loop
void updateFlagSet(Game theGame, bool cond);                // Refresh GUI if set to true
bool updateFlagGet(Game theGame);
void setLocalID(Game theGame, int id);                        // Set playerIDlocal
int getLocalID(Game theGame);                                 // Get playerIDlocal
int destroyGame(Game theGame);                                // Cleanup and destroy the application
int gameGetPlayerAmount(Game theGame);                        // Gets amount of players
void gameSetPlayerAmount(Game theGame, int amount);           // Sets amount of players
void gameSetInvulFlag(Game theGame, int id, bool cond);       // Set flag for invulnerability check
bool gameGetInvulFlag(Game theGame, int id);
#endif