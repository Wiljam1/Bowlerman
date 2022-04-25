#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "player.h"
#include "collissionDetection.h"
#include "sorter.h"

#define PUBLIC /* empty */
#define PRIVATE static
#define LENGTH 100

#define PLAYERAMOUNT 4    //how many players are online
int playerID=0;        //the players ID. 

const int WIDTH = 800; 
const int HEIGHT = 450;

struct game_type
{
    SDL_Window  *window;
    SDL_Surface *window_surface;

    //Player
    Player player[PLAYERAMOUNT];

    //Renderer
    SDL_Renderer *renderer;

    //Images
    SDL_Texture *background;
    SDL_Texture *player_texture[4];     //4  players, måste stå 4 annars blir de segmentation fault.
    
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
bool checkEvents(Game theGame)
{
    //Enter game loop (SDL_PollEvent)
    bool done = false;

    while(SDL_PollEvent(&theGame->window_event)){
        SDL_Event event = theGame->window_event;

        switch(event.type){
            case SDL_QUIT:
                done = true;
                break;
            case SDL_WINDOWEVENT_CLOSE:
                if(theGame->window){
                    SDL_DestroyWindow(theGame->window);
                    theGame->window = NULL;
                    done = true;
                }
            break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode){
                    case SDLK_ESCAPE:
                        done = true;  //Doesn't do anything right now
                    break;
                    default: break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.scancode)
                {
                // case SDL_SCANCODE_W: case SDL_SCANCODE_UP:
                //     theGame->player[playerID].up = 0;
                //     break;
                default:
                    break;
                }
                break;
        }
    }

    //Manage movement inputs
    manageMovementInputs(theGame);

    // send and retrive positions via server

    return done;
}

void manageMovementInputs(Game theGame)
{
    // Om du ska fortsätta göra movement bättre; 
    // https://stackoverflow.com/questions/39929853/priority-when-2-keys-are-pressed-at-the-same-time-script-for-a-game
    // Det är samma princip men mycket mer if-satser för att täcka alla fall av samtidiga knapptryck.
    int velX = 0, velY = 0;
    static int currentDirection = 0;
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_A] && state[SDL_SCANCODE_D]){
        if(currentDirection == 1)
            velX = -theGame->player[playerID].speed;
        else if(currentDirection == -1)
            velX = theGame->player[playerID].speed;
    }
    else if(state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D]){
        currentDirection = -1;
        velX = -theGame->player[playerID].speed;
    }
    else if(state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_A]){
        currentDirection = 1;
        velX = theGame->player[playerID].speed;
    }
    if(velX == 0){
        if(state[SDL_SCANCODE_W] && state[SDL_SCANCODE_S]){
            if(currentDirection == 2)
                velY = theGame->player[playerID].speed;
            else if(currentDirection == 3)
                velY = -theGame->player[playerID].speed;
        }
        else if(state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_S]){
            currentDirection = 2;
            velY = -theGame->player[playerID].speed;
        }
        else if(state[SDL_SCANCODE_S] && !state[SDL_SCANCODE_W]){
            currentDirection = 3;
            velY = theGame->player[playerID].speed;
        }
    }

    updatePlayerPos(theGame, playerID, velX, velY);
}

void updatePlayerPos(Game theGame, int playerID, int velX, int velY)
{
    // update (client-side) player positions
    theGame->player[playerID].xPos += velX;
    theGame->player[playerID].yPos += velY;
}

//initializes startvalues for game
void initGame(Game theGame)
{
    //loads in textures
    theGame->background = (SDL_Texture *) loadTextures(theGame, "alley.png");
    theGame->player_texture[0] = (SDL_Texture *) loadTextures(theGame, "bowlermantestskins/bowman00.png");
    theGame->player_texture[1] = (SDL_Texture *) loadTextures(theGame, "pin2.png");
    theGame->player_texture[2] = (SDL_Texture *) loadTextures(theGame, "pin2.png");
    theGame->player_texture[3] = (SDL_Texture *) loadTextures(theGame, "pin2.png");
    SDL_FreeSurface(theGame->window_surface);

    //check server what ID you have.
    //getPlayerID();


    //inits x-amount of players
    theGame->player[0] = initPlayer(5, 5);   //sets x and y coordinates and resets values.
    //initPlayerRect(theGame); //inits playerRect[0] to position of player0
    
    if(PLAYERAMOUNT>1){
        theGame->player[1] = initPlayer(750, 300);   //sets x and y coordinates and resets values.
    }
    if(PLAYERAMOUNT>2){
        theGame->player[2] = initPlayer(0, 300);   //sets x and y coordinates and resets values.
    }
    if(PLAYERAMOUNT>3){
        theGame->player[3] = initPlayer(750, 0);   //sets x and y coordinates and resets values.
    }
  
    // //get and scale the dimensions of texture (based on how many players are online)
    // for(int i=0; i<PLAYERAMOUNT; i++)
    // {
    //     SDL_QueryTexture(theGame->player_texture[i], NULL, NULL, &playerRect[i].w, &playerRect[i].h);
    //     playerRect[i].w /=7;              //scales down width by 4
    //     playerRect[i].h /=7;              //scales down height by 4  
    // }

}

//game loop
PUBLIC void gameUpdate(Game theGame) 
{
    Player player[PLAYERAMOUNT];   //declares x-ammounts of players depending on "playerAmmount"
    initGame(theGame); //initializes startvalues. coordinates etc.
    //int renderOrder[4]={0,1,2,3}; //what order to render players

    //gameloop:
    bool done = false;
    while(!done)
    {
        //Check for events
        done = checkEvents(theGame);

        //Process events (time based stuff)

        //Collisiondetection

        //Send/receive data to server

        //render display
        renderTextures(theGame); 

        SDL_Delay(10); //man behöver ta minus här för att räkna in hur lång tid spelet tar att exekvera
    }
}

//loads media into texture
PUBLIC SDL_Texture *loadTextures(Game newGame, char fileLocation[])   //loadmedia
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
void renderTextures(Game theGame)
{
    SDL_RenderClear(theGame->renderer); //clear renderer
    //updates/renders background
    SDL_RenderCopy(theGame->renderer, theGame->background, NULL, NULL);


    //bubble-sort the players y-position into the array "renderOrder"
    //arraySorter(player, PLAYERAMOUNT, renderOrder);


    // renders players
    for(int i=0; i<PLAYERAMOUNT; i++){
        SDL_Rect rect = {theGame->player[i].xPos, theGame->player[i].yPos, theGame->player->width, theGame->player->height};
        SDL_RenderCopyEx(theGame->renderer, theGame->player_texture[i], 0, &rect, 0, NULL, 0);
    }
    
    SDL_RenderPresent(theGame->renderer); //present renderer
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