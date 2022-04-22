#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "player.h"
#include "collissionDetection.h"

#define PUBLIC /* empty */
#define PRIVATE static
#define LENGTH 100

int playerAmmount=4;    //how many players are online
int playerID=0;        //the players ID. 

const int WIDTH = 800; 
const int HEIGHT = 450;


PRIVATE void updateMedia(Game newGame, SDL_Rect playerRect[]);
PRIVATE void createGameMedia(Game newGame);

struct game_type
{
    SDL_Window  *window;
    SDL_Surface *window_surface;

    //Renderer
    SDL_Renderer *renderer;

    //Images
    SDL_Texture *background;
    SDL_Texture *player_texture[3];     //4  players
    
    SDL_Event    window_event;
}; 

//initializes game
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

//handles processes, like keyboard-inputs etc
int checkEvents(Game newGame, Player *player)
{
    bool keep_window_open = true;
    while(SDL_PollEvent(&newGame->window_event) > 0)
    {
        SDL_Event event = newGame->window_event;
        
        switch(newGame->window_event.type)
        {
            case SDL_QUIT:
                keep_window_open = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    player[playerID]->up = 1;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    player[playerID]->left = 1;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    player[playerID]->down = 1;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    player[playerID]->right = 1;
                    break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    player[playerID]->up = 0;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    player[playerID]->left = 0;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    player[playerID]->down = 0;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    player[playerID]->right = 0;
                    break;
                }
                break;
        }
    }

    // determine velocity of player
    determinePlayerVelocity(player[playerID]);

    // update (client-side) player positions
    updatePlayerClientPosition(player[playerID]);

    // send and retrive positions via server

    //check if within bounds. write in: collissiondetection.c

    // set the positions of the player in the struct
    playerRect[playerID].x = (int) player[playerID]->xPos;
    playerRect[playerID].y = (int) player[playerID]->yPos;


    return keep_window_open;
}

//initializes startvalues for game
PRIVATE void initGame(Game newGame, Player *player)
{
    //loads in textures
    newGame->background = (SDL_Texture *) loadMedia(newGame, "grass00.bmp");
    SDL_FreeSurface(newGame->window_surface);
    newGame->player_texture[0] = (SDL_Texture *) loadMedia(newGame, "pin2.png");
    SDL_FreeSurface(newGame->window_surface);
    newGame->player_texture[1] = (SDL_Texture *) loadMedia(newGame, "pin2.png");
    SDL_FreeSurface(newGame->window_surface);
    newGame->player_texture[2] = (SDL_Texture *) loadMedia(newGame, "pin2.png");
    SDL_FreeSurface(newGame->window_surface);
    newGame->player_texture[3] = (SDL_Texture *) loadMedia(newGame, "pin2.png");
    SDL_FreeSurface(newGame->window_surface);

    //check server what ID you have.
    //getPlayerID();


    //inits x-ammount of players based on "playerAmmount"-variable (global variable)
    player[0] = initPlayer(00, 0);   //sets x and y coordinates and resets values.
    initPlayerRect(&playerRect[0], player[0]); //inits playerRect[0] to position of player0
    
    if(playerAmmount>1){
        player[1] = initPlayer(50, 50);   //sets x and y coordinates and resets values.
        initPlayerRect(&playerRect[1], player[1]); //inits playerRect[0] to position of player0
    }
    if(playerAmmount>2){
        player[2] = initPlayer(80, 80);   //sets x and y coordinates and resets values.
        initPlayerRect(&playerRect[2], player[2]); //inits playerRect[0] to position of player0
    }
    if(playerAmmount>3){
        player[3] = initPlayer(100, 100);   //sets x and y coordinates and resets values.
        initPlayerRect(&playerRect[3], player[3]); //inits playerRect[0] to position of player0
    }

  
    //get and scale the dimensions of texture (based on how many players are online)
    for(int i=0; i<playerAmmount; i++)
    {
        SDL_QueryTexture(newGame->player_texture[i], NULL, NULL, &playerRect[i].w, &playerRect[i].h);
        playerRect[i].w /=7;              //scales down width by 4
        playerRect[i].h /=7;              //scales down height by 4  
    }
    

    

}

//game loop
PUBLIC void gameUpdate(Game newGame) 
{
    Player player[playerAmmount-1];   //declares x-ammounts of players depending on "playerAmmount"
    initGame(newGame, player); //initializes startvalues. coordinates etc.


    //gameloop:
    bool keep_window_open = true;
    while(keep_window_open)
    {
        //Check for events
        keep_window_open = checkEvents(newGame, player);
        
        //handles events
        //handleEvents();

        //renders to screen
        updateMedia(newGame, playerRect); 

        SDL_Delay(10); //man behöver ta minus här för att räkna in hur lång tid spelet tar att exekvera
    }
}

//loads media into texture
PUBLIC SDL_Texture *loadMedia(Game newGame, char fileLocation[])   //loadmedia
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

//renders background and players etc.
PRIVATE void updateMedia(Game newGame, SDL_Rect playerRect[])
{
    SDL_RenderClear(newGame->renderer); //clear renderer

    //updates/renders background
    SDL_RenderCopy(newGame->renderer, newGame->background, NULL, NULL);

    //bubble sort of players x-position
    // renders players. Depends on global variable "playerAmmount"
    for(int i=0; i<playerAmmount; i++){
        SDL_RenderCopy(newGame->renderer, newGame->player_texture[i], NULL, &playerRect[i]);
    }
    
    SDL_RenderPresent(newGame->renderer); //present renderer
}

PUBLIC void destroyGame(Game theGame)
{
    SDL_DestroyTexture(theGame->background);
    SDL_DestroyTexture(theGame->player_texture[0]);
    SDL_DestroyTexture(theGame->player_texture[1]);
    SDL_DestroyTexture(theGame->player_texture[2]);
    SDL_DestroyTexture(theGame->player_texture[3]);
    SDL_DestroyRenderer(theGame->renderer);
    SDL_DestroyWindow(theGame->window);
    SDL_Quit();
}