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
#include "network.h"
#include "powerup.h"
#include "sounds.h"
#include "gui.h"
#include "tcpclient.h"

#define PUBLIC /* empty */
#define PRIVATE static
#define LENGTH 100

void initGame(Game theGame, UDPData *udpData, UDPInit *udpValues);

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
void initGame(Game theGame, UDPData *udpData, UDPInit *udpValues)
{
    //inits SDL-net and loads in correct IP-adresses etc.
    initSDLNet(udpValues);
    // Init sounds
    initSounds();
    //Init random seed
    srand(time(NULL));
    // Loading textures from file
    loadAllTextures(theGame);

    // get playerID via UDP and saves it in theGame->playerIDLocal
    getPlayerIDviaUDP(theGame, udpData, udpValues);
    
    //Kollar player-ammount (hur många spelare som är online). Just nu är den satt till att alltid vara 4.
    checkPlayerAmmount(theGame);

    //initierar spelare
    initAllPlayers(theGame);
    
    //initierar väggar
    initAllWalls(theGame);

    //Init GUI
    initGUI(theGame);

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
                SDL_DestroyWindow(theGame->window);
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
    Player player[MAXPLAYERS]; // declares x-amounts of players
    UDPInit udpValues = SetUDPValues();     //returns a struct for udp-init-struct. Like IP-adress etc.
    UDPData udpData = UDPDataTransfer();    //Resets data struct, Like player x,y -positions etc.
    initGame(theGame, &udpData, &udpValues);         // initializes startvalues. coordinates etc.
    Sounds sounds = initSoundFiles();
    // Game Loop:
    bool done = false;
    while (!done)
    {
        // Check for events
        done = checkEvents(theGame);
        
        // Process events (time based stuff)
        process(theGame, &sounds);
        playBackroundMusic(&sounds);

        // Collisiondetection
        collisionDetect(theGame, &sounds);
        

        // Send/receive data to server
        manageUDP(theGame, &udpData, &udpValues);

        // Update GUI labels
        updateGUI(theGame);

        // render display
        renderTextures(theGame);

        SDL_Delay(theGame->delayInMS); // man behöver ta minus här för att räkna in hur lång tid spelet tar att exekvera
    }
    //destroySoundFiles(sounds);
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
    SDL_DestroyRenderer(theGame->renderer);
    SDL_DestroyWindow(theGame->window);
    SDL_Quit();
}
