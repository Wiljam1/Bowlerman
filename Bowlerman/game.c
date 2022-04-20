#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "player.h"

#define PUBLIC /* empty */
#define PRIVATE static
#define LENGTH 100

const int WIDTH = 800; 
const int HEIGHT = 450;

PRIVATE void updateMedia(Game newGame);
PRIVATE void updateAllMedia(Game newGame);
PRIVATE void updateBackground(Game newGame);
PRIVATE void createGameMedia(Game newGame);

struct game_type
{
    SDL_Window  *window;
    SDL_Surface *window_surface;

    //Renderer
    SDL_Renderer *renderer;

    //Images
    SDL_Texture *background;
    SDL_Texture *player_texture;
    
    SDL_Event    window_event;
}; 

PRIVATE void createGameMedia(Game newGame){
    newGame->background = (SDL_Texture *) loadBackground(newGame, "grass00.bmp");
    SDL_FreeSurface(newGame->window_surface);
    newGame->player_texture = (SDL_Texture *) loadBackground(newGame, "pin2.png");
    SDL_FreeSurface(newGame->window_surface);
}

PUBLIC Game createGame()
{
    Game newGame = malloc(sizeof(struct game_type));
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Failed to initialize SDL2: %s\n", SDL_GetError());
    }
    newGame->window= SDL_CreateWindow("Bowlerman 0.1", 
                                       SDL_WINDOWPOS_CENTERED, 
                                       SDL_WINDOWPOS_CENTERED, 
                                       WIDTH, HEIGHT, 
                                       SDL_WINDOW_SHOWN);
    
    newGame->renderer = SDL_CreateRenderer(newGame->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    newGame->window_surface = SDL_GetWindowSurface(newGame->window);

    return newGame;
}

int processEvents(Game newGame)
{
    bool keep_window_open = true;
    while(SDL_PollEvent(&newGame->window_event) > 0)
    {
        SDL_Event test = newGame->window_event;
        
        switch(newGame->window_event.type)
        {
            case SDL_QUIT:
                keep_window_open = false;
                break;
        }
    }
    return keep_window_open;
}

PUBLIC void gameUpdate(Game newGame) //game loop
{
    createGameMedia(newGame); //loads in textures
    Player player1 = initPlayer(500, 500);   //x and y coordinates
    SDL_Rect playerRect;
    playerRect.h = getPlayerHeight();
    playerRect.w = getPlayerWidth();
    playerRect.y = getPlayerYPosition(player1);
    playerRect.x = getPlayerYPosition(player1);


    //gameloop:
    bool keep_window_open = true;
    while(keep_window_open)
    {
        //Check for events
        keep_window_open = processEvents(newGame);

        //updates all renders
        updateAllMedia(newGame); 
        SDL_Delay(10); //man behöver ta minus här för att räkna in hur lång tid spelet tar att exekvera
    }
}

PUBLIC SDL_Texture *loadBackground(Game newGame, char fileLocation[])   //loadmedia
{
    bool success = true;
    char fileLocationInResources[100]="resources/";
    strcat(fileLocationInResources, fileLocation);
    newGame->window_surface = IMG_Load(fileLocationInResources);
    if(newGame->window_surface == NULL)
    {
        printf("Failed to load surface! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    return SDL_CreateTextureFromSurface(newGame->renderer, newGame->window_surface);
}

//  KLAR funktion!
PUBLIC int loadMedia(Game newGame, char fileLocation[])
{
    bool success = true;
    char fileLocationInResources[100]="resources/";
    strcat(fileLocationInResources, fileLocation);
    newGame->window_surface = IMG_Load(fileLocationInResources);
    if(newGame->window_surface == NULL)
    {
        printf("Failed to load surface! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    newGame->background = SDL_CreateTextureFromSurface(newGame->renderer, newGame->window_surface);

    return success;
}

//updates all renders: background and players etc.
PRIVATE void updateAllMedia(Game newGame)
{
    SDL_RenderClear(newGame->renderer); //clear renderer

    updateBackground(newGame);
    updateMedia(newGame);

    SDL_RenderPresent(newGame->renderer); //present renderer
}

PRIVATE void updateBackground(Game newGame)
{
    SDL_RenderCopy(newGame->renderer, newGame->background, NULL, NULL);
}

PRIVATE void updateMedia(Game newGame)
{
    SDL_RenderCopy(newGame->renderer, newGame->background, NULL, NULL);
    //SDL_RenderCopy(newGame->renderer, newGame->player_texture, destination, NULL);
}

PUBLIC void destroyGame(Game theGame)
{
    SDL_DestroyTexture(theGame->background);
    SDL_DestroyTexture(theGame->player_texture);
    SDL_DestroyRenderer(theGame->renderer);
    SDL_DestroyWindow(theGame->window);
    SDL_Quit();
}