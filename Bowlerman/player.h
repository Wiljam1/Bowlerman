#ifndef player_h
#define player_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

struct playerMovement;
struct playerController;

//det är denna strukt vi sen skickar via UDP. struct för server
typedef struct playerController *Player;
Player initPlayer(float xPos, float yPos);

float getPlayerXPosition(Player p);
float getPlayerYPosition(Player p);
int getPlayerHeight();
int getPlayerWidth();


//strukt för clienten. Denna skickas inte via UDP
//denna strukt funkade inte när jag hade den i player.c jättekonstigt
struct playerMovement
{
    int up;
    int down;
    int left;
    int right;
    float yVel;
    float xVel;
    float xPos;
    float yPos;
    float xPosOld;
    float yPosOld;
};

/*struct  för up, down, right, left */
typedef struct playerMovement *PlayerMovement;

//ändrar spelarens (clientmässiga) hastighet beroende på användarens tangentbordsintryckningar
void determinePlayerVelocity(PlayerMovement playerMoving);

//ändrar spelarens (clientmässiga) position
void updatePlayerClientPosition(PlayerMovement playerMoving);




//resets playerMoving to 0
void resetPlayerMoving(PlayerMovement playerMoving);

//rect for player
SDL_Rect playerRect[4]; 
//inits a player_rect with the x-and-y-position of player
void initPlayerRect(SDL_Rect *playerRect, Player player1); 

//void playerMoveRight(Player player1);
//void playerMoveLeft(Player player1);
//void playerMoveDown(Player player1);
//void playerMoveUp(Player player1);


//void destroyPlayer();

#endif