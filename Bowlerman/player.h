#ifndef player_h
#define player_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"

typedef struct playerController *Player;

//när jag har denna struct i player.c funkar koden inte i game.c jättekonstigt! fattar ej varför
//jag tror det är för att programet måste veta hur stor struct:en är (?)
struct playerController
{
    int id;   //för packets via UDP behöver man kunna veta vem det är som skickar datan.
    bool up;
    bool down;
    bool left;
    bool right;
    float yVel;
    float xVel;
    float xPos;
    float yPos;
    float xPosOld;
    float yPosOld;

    float speed;
    char name[40];
    
    //SDL_Rect playerRectangle;   //struct to hold the position and size of the sprite
};

//initiate player position with x and y coordinates
Player initPlayer(float xPos, float yPos);

float getPlayerXPosition(Player p);
float getPlayerYPosition(Player p);
int getPlayerHeight();
int getPlayerWidth();


//ändrar spelarens (clientmässiga) hastighet beroende på användarens tangentbordsintryckningar
void determinePlayerVelocity(Player playerMoving);

//ändrar spelarens (clientmässiga) position
void updatePlayerClientPosition(Player playerMoving);


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