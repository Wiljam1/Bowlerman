#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "collissionDetection.h"
#include "bomb.h"
#include "textures.h"
#include "wall.h"
#include "udpClient.h"
#include "tcpClient.h"
#include "powerup.h"
#include "sounds.h"
#include "gui.h"

#define PUBLIC /* empty */
#define PRIVATE static
#define LENGTH 100
void menu();
void initGame(Game theGame, UDPData *udpData, UDPStruct *udpValues, bool *done);

// initializes game-window
PUBLIC Game createWindow()
{
    Game theGame = malloc(sizeof(struct game_type));
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Failed to initialize SDL2: %s\n", SDL_GetError());
    }
    theGame->window = SDL_CreateWindow("Bowlerman 0.1",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WIDTH, HEIGHT,
                                       SDL_WINDOW_SHOWN);

    theGame->renderer = SDL_CreateRenderer(theGame->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    theGame->window_surface = SDL_GetWindowSurface(theGame->window);
    return theGame;
}

// initializes startvalues for game
void initGame(Game theGame, UDPData *udpData, UDPStruct *udpValues, bool *done)
{
    // Loading textures from file
    loadAllTextures(theGame);
    //inits SDL-net and loads in correct IP-adresses etc.
    initSDLNet();
     //Init GUI
    initGUI(theGame);
    //Menu loop
    menu(theGame, done);

    initUDP(udpValues);
    //TCPstruct tcpValues = createTCPstruct();     //returns a struct for tcp-init-struct.	
	//initTCP(&tcpValues);            //initiates TCP
	//manageTCP(&tcpValues);          //starts TCP
	//closeTCP(&tcpValues);           //closes TCP

    // Init sounds
    initSounds();
    //Init random seed
    srand(time(NULL));
    

    // get playerID via UDP and saves it in theGame->playerIDLocal
    getPlayerIDviaUDP(theGame, udpData, udpValues);
    
    //Kollar player-ammount (hur många spelare som är online). Just nu är den satt till att alltid vara 4.
    checkPlayerAmmount(theGame);

    //initierar spelare
    initAllPlayers(theGame);
    
    //initierar väggar
    initAllWalls(theGame);

   // initGUI(theGame);

    //set game-delay to x-miliseconds. You should have lower delay if you have a slower computer
    theGame->delayInMS=10;
}

// handles processes, like keyboard-inputs etc
bool checkEvents(Game theGame)
{
    // Manage movement inputs
    manageMovementInputs(theGame);
    
    // Enter game loop (SDL_PollEvent)
    bool done = false;
    while (SDL_PollEvent(&theGame->window_event))
    {
        SDL_Event event = theGame->window_event;
        switch (event.type)
        {
        case SDL_QUIT:
            done = true;
            break;
        case SDL_WINDOWEVENT_CLOSE:
            if (theGame->window)
            {
                theGame->window = NULL;
                done = true;
            }
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_SPACE:
                tryToPlaceBomb(theGame, theGame->playerIDLocal);
                break;
            case SDLK_ESCAPE:
                done = true;
                break;
            case SDLK_t:
                //Testing
                playerAddSpeed(&theGame->player[theGame->playerIDLocal], 0.5);
                printf("Speed is now: %d\n", getPlayerSpeed(theGame->player[theGame->playerIDLocal]));
                break;
            case SDLK_y:
                //Testing
                playerAddExplosionPower(&theGame->player[theGame->playerIDLocal], 1);
                printf("Power is now: %d\n", theGame->player[theGame->playerIDLocal].explosionPower);
                break;
            case SDLK_u:
                //Testing
                playerAddAmountOfBombs(&theGame->player[theGame->playerIDLocal], 1);
                printf("Bombs is now: %d\n", theGame->player[theGame->playerIDLocal].amountOfBombs);
                break;
            case SDLK_i:
                playerAddLives(&theGame->player[theGame->playerIDLocal], 1);
                printf("You have now: %d lives\n", theGame->player[theGame->playerIDLocal].noOfLives);
            default:
                break;
            }
        default:
            break;
        }
    }
    return done;
}

void manageMovementInputs(Game theGame)
{
    int velX = 0, velY = 0;
    int id = theGame->playerIDLocal;
    char direction;
    Player player = theGame->player[theGame->playerIDLocal];
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (player.isInvulnerable == false || player.isDead == false)
    {
        if (state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_S])
        {
            velX = -getPlayerSpeed(player);
            direction = 'a';
        }
        else if (state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_S])
        {
            velX = getPlayerSpeed(player);
            direction = 'd';
        }
        if (velX == 0)
        {
            if (state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_S])
            {
                velY = -getPlayerSpeed(player);
                direction = 'w';
            }
            else if (state[SDL_SCANCODE_S] && !state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_D])
            {
                velY = getPlayerSpeed(player);
                direction = 's';
            }
        }
    }
    if (!velX && !velY) {
        direction = '0';
    }
    // Update player positions
    updateMovementDirection(theGame, id, direction);
    updatePlayerXPosition(theGame, id, velX);
    updatePlayerYPosition(theGame, id, velY);
}

// game loop
PUBLIC void gameUpdate(Game theGame)
{
    // Initialize
    bool done = false;
    Player player[MAXPLAYERS]; // declares x-amounts of players
    UDPStruct udpValues = createUDPstruct();     //returns a struct for udp-init-struct. Like IP-adress etc.
    UDPData udpData = UDPDataReset();    //Resets data struct, Like player x,y -positions etc.
    initGame(theGame, &udpData, &udpValues, &done);         // initializes startvalues. coordinates etc.
    Sounds sounds = initSoundFiles();
    // Game Loop:

    while (!done)
    {
        playBackroundMusic(&sounds);
        // Check for events
        
    
        // Process events (time based stuff)
        process(theGame, &sounds);

        done = checkEvents(theGame);

        // Collisiondetection
        collisionDetect(theGame, &sounds);
        // Send/receive data to server
        manageUDP(theGame, &udpData, &udpValues);

        // Update GUI labels (only updates when updateFlag = true)
        updateGUI(theGame); //behövs göras om, mem leak (mem leak löst med flagga temporärt)

        // render display
        renderTextures(theGame);

        SDL_Delay(theGame->delayInMS); // man behöver ta minus här för att räkna in hur lång tid spelet tar att exekvera
    }
    destroySoundFiles(sounds);
}
void menu(Game theGame, bool *done)
{
    bool loop = true;
    SDL_Color black = {0, 0, 0, 0};
    SDL_Rect backRect = {0, 0, WIDTH, HEIGHT};
    SDL_RenderCopy(theGame->renderer, theGame->background, NULL, &backRect);
    
    theGame->window_surface = TTF_RenderText_Blended(theGame->font, "1) HOST SERVER", black);
    SDL_Texture *menuT1 = SDL_CreateTextureFromSurface(theGame->renderer, theGame->window_surface);
    theGame->window_surface = TTF_RenderText_Blended(theGame->font, "2) JOIN SERVER", black);
    SDL_Texture *menuT2 = SDL_CreateTextureFromSurface(theGame->renderer, theGame->window_surface);
    theGame->window_surface = TTF_RenderText_Blended(theGame->font, "3) QUIT GAME", black);
    SDL_Texture *menuT3 = SDL_CreateTextureFromSurface(theGame->renderer, theGame->window_surface);
    SDL_FreeSurface(theGame->window_surface);

    SDL_Rect textRect1 = {200, 200, 400, 100};
    SDL_Rect textRect2 = {200, 300, 400, 100};
    SDL_Rect textRect3 = {200, 400, 400, 100}; 
    SDL_RenderCopy(theGame->renderer, menuT1, NULL, &textRect1);
    SDL_RenderCopy(theGame->renderer, menuT2, NULL, &textRect2);
    SDL_RenderCopy(theGame->renderer, menuT3, NULL, &textRect3);

    SDL_RenderPresent(theGame->renderer); // present renderer
    
    while (loop)
    {
        while(SDL_PollEvent(&theGame->window_event))
        {
            SDL_Event event = theGame->window_event;
            switch(event.type)
            {
                case SDL_QUIT:
                    *done = true;
                    loop = false;
                    break;
                case SDL_WINDOWEVENT_CLOSE:
                    if(theGame->window)
                    {
                        theGame->window = NULL;
                        *done = true;
                        loop = false;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_1:
                            printf("\nHOST SERVER\n");
                            SDL_Delay(1000);
                            *done = false;
                            loop = false;
                            break;
                        case SDLK_2:
                            printf("\nJOIN SERVER\n");
                            *done = false;
                            loop = false;
                            break;
                        case SDLK_3:
                            printf("\nQUIT GAME\n");
                            *done = true;
                            loop = false;
                            break;
                    }
            }  
        }
        SDL_Delay(1000/60);
    }
    SDL_DestroyTexture(menuT1);
    SDL_DestroyTexture(menuT2);
    SDL_DestroyTexture(menuT3);
}
// renders background and players etc.
PUBLIC void destroyGame(Game theGame)
{
    SDL_DestroyTexture(theGame->background);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            SDL_DestroyTexture(theGame->player_texture[i][j]);
        }
    }
    for (int i = 0; i < 4; i++)
    {
        SDL_DestroyTexture(theGame->bomb_texture[i]);
    }
    for(int i = 0; i < 3; i++){
        SDL_DestroyTexture(theGame->textureWall[i]);
    }
    SDL_DestroyTexture(theGame->bombExplosion_texture);
    destroyGUI(theGame);
    SDLNet_Quit();
    SDL_DestroyRenderer(theGame->renderer);
    SDL_DestroyWindow(theGame->window);
    SDL_Quit();
    //free(theGame);
}
