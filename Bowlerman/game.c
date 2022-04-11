#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"

#define PUBLIC /* empty */
#define PRIVATE static

const int HEIGHT = 1280;
const int WIDTH = 720;

struct game_type
{
    SDL_Window  *window;
    SDL_Surface *window_surface;
    SDL_Renderer *renderer;
    SDL_Event    window_event;
};

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
                                       HEIGHT, WIDTH, 
                                       SDL_WINDOW_SHOWN);
    
    newGame->renderer = SDL_CreateRenderer(newGame->window, -1, 0);
    newGame->window_surface = SDL_GetWindowSurface(newGame->window);
    return newGame;
}

PUBLIC void gameUpdate(Game newGame)
{
    bool keep_window_open = true;
    while(keep_window_open)
    {
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
    }
}


PUBLIC void destoryGame(Game theGame)
{
    SDL_DestroyRenderer(theGame->renderer);
    SDL_FreeSurface(theGame->window_surface);
    SDL_DestroyWindow(theGame->window);
    SDL_Quit();
}