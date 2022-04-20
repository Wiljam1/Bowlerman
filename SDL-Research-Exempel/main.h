#ifndef main_h
#define main_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define WIDTH 1280
#define HEIGHT 720
#define TICK 0.1f
#define SPEED 3
#define DIAGSPEED 2.2
#define WALLCOUNT 100
#define WALLSIZE 64
#define MANSIZE 48

#define STRSIZE 100

#define STATUS_STATE_INTRO 0
#define STATUS_STATE_GAME 1
#define STATUS_STATE_GAMEOVER 2

//const int WIDTH = 1280, HEIGHT = 720;

typedef struct 
{
    int x, y;
    short life;
    char *name;
    bool isDead, isMoving, isMovingLeft, isMovingRight, isMovingDiagonal;
    
    //Man collisionBox!
    SDL_Rect collisionBox;
}   Man;

typedef struct
{
    int x, y, w, h;

    //Wall collisionBox!
    SDL_Rect collisionBox;
} Wall;

typedef struct
{
    int x, y, w, h;
} Brick;

typedef struct
{
    //Players
    Man man;

    //Objects
    Wall walls[WALLCOUNT];
    Brick bricks[WALLCOUNT];

    //Images
    SDL_Texture *wall;  //Gör en array av bilderna
    SDL_Texture *brick;
    SDL_Texture *alley;
    SDL_Texture *pin;
    SDL_Texture *blood;
    SDL_Texture *manFrames[4];
    SDL_Texture *label;
    int labelW, labelH;

    //Fonts
    TTF_Font *font;

    //time
    int time;
    int statusState;

    //Sounds

    //Renderer
    SDL_Renderer *renderer;

} GameState;

SDL_Texture *loadImage(GameState *game, SDL_Surface *surface, char imgLocation[STRSIZE]);
void loadGame(GameState *game);
bool processEvents(SDL_Window *window, GameState *game);
void process(GameState *game);
bool checkCollision(SDL_Rect a, SDL_Rect b);
int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2);
void collisionDetect(GameState *game);
void doRender(SDL_Renderer *renderer, GameState *game);
void closeGame(GameState *game, SDL_Window *window);


#endif