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
    
    SDL_Rect playerRect;   //struct to hold the position and size of the sprite
    char moveDirection;  // Keeps track of player movement direction for sprite rendering
};

struct playerSprites
{
    SDL_Rect BowlerManVert[8];
    SDL_Rect BowlerManHori[8];
};
typedef struct playerSprites PlayerSprites;
//initiate player position with x and y coordinates
PlayerSprites GetPlayerSprite();
Player initPlayer(float xPos, float yPos, int playerID);

float getPlayerXPosition(Player p);
float getPlayerYPosition(Player p);
int getPlayerHeight();
int getPlayerWidth();
int getPlayerSpeed(Player p);
int getPlayerID(Player p);


//ändrar spelarens (clientmässiga) hastighet beroende på användarens tangentbordsintryckningar
// void determinePlayerVelocity(Player playerMoving);

//ändrar spelarens (clientmässiga) position
//void playerPositionUpdate(int x, int y);

//inits a player_rect with the x-and-y-position of player
void initPlayerRect(SDL_Rect *playerRect, Player player1); 

//void destroyPlayer();

#endif