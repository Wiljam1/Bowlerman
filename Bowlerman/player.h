#ifndef player_h
#define player_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

#define PUBLIC /* empty */
#define PRIVATE static

// #define PLAYERHEIGHT = 100;
// #define PLAYERWIDTH = 50;    //Använder structen för sånt här istället
// #define PLAYERSPEED = 5;     // speed in pixels/second

//nånting konstigt med player.h elr player.c, tror att de blir inkluderade två gånger
//Får en massa errors när man flyttar runt saker här "already defined blah blah blah"

typedef struct playerController Player;

//när jag har denna struct i player.c funkar koden inte i game.c jättekonstigt! fattar ej varför
//jag tror det är för att programmet måste veta hur stor struct:en är (?)
struct playerController
{
    int id;   //för packets via UDP behöver man kunna veta vem det är som skickar datan.
    bool up, down, left, right;
    float yVel, xVel, xPos, yPos;
    float xPosOld;
    float yPosOld;

    float speed;
    int height;
    int width;
    char name[40];
    
    //SDL_Rect playerRect;   //struct to hold the position and size of the sprite
};

struct playerSprites
{
    SDL_Rect redMan[3][8];
    SDL_Rect blueMan[3][8];
    SDL_Rect purpleMan[3][8];
    SDL_Rect yellowMan[3][8];
};
typedef struct playerSprites PlayerSprites;
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

//inits a player_rect with the x-and-y-position of player
void initPlayerRect(SDL_Rect *playerRect, Player player1); 

//void playerMoveRight(Player player1);
//void playerMoveLeft(Player player1);
//void playerMoveDown(Player player1);
//void playerMoveUp(Player player1);


//void destroyPlayer();

#endif