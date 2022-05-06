#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "collissionDetection.h"
#include "bomb.h"
#include "textures.h"
#include "wall.h"
#include "network.h"
#include "powerup.h"

#define PUBLIC /* empty */
#define PRIVATE static
#define LENGTH 100

void initGame(Game theGame, UDPData *udpData, UDPInit *udpValues);

// initializes game
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
    
    initSDLNet(udpValues);
    
    // Loading textures from file
    loadAllTextures(theGame);

    // check server what ID you have.
    // getPlayerID();
    // get playerID via UDP

    // 1st: send info to UDP-server
    memcpy(udpValues->p->data, &(*udpData), sizeof(UDPData) + 1);
    udpValues->p->len = sizeof(UDPData) + 1;
    udpValues->p->address.host = udpValues->srvadd.host; /* Set the destination host */
    udpValues->p->address.port = udpValues->srvadd.port; /* And destination port */
    SDLNet_UDP_Send(udpValues->sd, -1, udpValues->p);
    // 2nd: receive info from UDP-server
    while (!SDLNet_UDP_Recv(udpValues->sd, udpValues->p2))
        ; // spin-lock tills received info from UDP-server
    memcpy(udpData, (char *)udpValues->p2->data, sizeof(UDPData));
    printf("crash");
    theGame->playerIDLocal = udpData->playerID;
    printf("UDP Packet incoming %d\n", udpData->playerID);
    printf("%d", theGame->playerIDLocal);
    // detta ska ändras via servern sen.
    theGame->playerAmount = PLAYERAMOUNT;

    // allow bomb placement init
    for (int i = 0; i < 4; i++)
    {
        theGame->allowBombPlacement[i] = 1;
    }

    initAllPlayers(theGame);
    
    initAllWalls(theGame);

    theGame->powerups[0].x = 0;
    theGame->powerups[0].y = 0;
    theGame->powerups[0].h = 70;
    theGame->powerups[0].w = 70;
  
}

// handles processes, like keyboard-inputs etc
bool checkEvents(Game theGame)
{
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
                playerAddSpeed(&theGame->player[theGame->playerIDLocal], 1);
                printf("Speed is now: %d\n", getPlayerSpeed(theGame->player[theGame->playerIDLocal]));
                //------
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode)
            {
            // case SDLK_w: case SDLK_UP:
            //     up = false;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
    // Manage movement inputs
    manageMovementInputs(theGame);

    return done;
}

void manageMovementInputs(Game theGame)
{
    int velX = 0, velY = 0;
    Player player = theGame->player[theGame->playerIDLocal];
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_S])
    {
        velX = -getPlayerSpeed(player);
        theGame->player[theGame->playerIDLocal].moveDirection = 'a';
    }
    else if (state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_S])
    {
        velX = getPlayerSpeed(player);
        theGame->player[theGame->playerIDLocal].moveDirection = 'd';
    }
    if (velX == 0)
    {
        if (state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_S])
        {
            velY = -getPlayerSpeed(player);
            theGame->player[theGame->playerIDLocal].moveDirection = 'w';
        }
        else if (state[SDL_SCANCODE_S] && !state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_D])
        {
            velY = getPlayerSpeed(player);
            theGame->player[theGame->playerIDLocal].moveDirection = 's';
        }
    }
    if (!velX && !velY)
        theGame->player[theGame->playerIDLocal].moveDirection = '0';
    // Update player positions
    theGame->player[theGame->playerIDLocal].xPos += velX;
    theGame->player[theGame->playerIDLocal].yPos += velY;
}

// game loop
PUBLIC void gameUpdate(Game theGame)
{
    // Initialize
    Player player[MAXPLAYERS]; // declares x-amounts of players
    UDPInit udpValues = SetUDPValues();
    UDPData udpData = UDPDataTransfer();
    initGame(theGame, &udpData, &udpValues);         // initializes startvalues. coordinates etc.

    // Game Loop:
    bool done = false;
    while (!done)
    {
        // Check for events
        done = checkEvents(theGame);
        
        // Process events (time based stuff)
        process(theGame);

        // Collisiondetection
        collisionDetect(theGame);
        testCollosionWithBombs(theGame);     //Alla dessa kan flyttas in i collisionDetect();
        testCollisionWithWalls(theGame);
        testCollosionWithExplosion(theGame);
        

        // Send/receive data to server
        manageUDP(theGame, &udpData, &udpValues);

        // render display
        renderTextures(theGame);

        SDL_Delay(1000 / 60); // man behöver ta minus här för att räkna in hur lång tid spelet tar att exekvera
    }
}

PUBLIC SDL_Texture *loadTextures(Game newGame, char fileLocation[]) // loadmedia
{
    bool success = true;
    char fileLocationInResources[100] = "resources/";
    strcat(fileLocationInResources, fileLocation);
    newGame->window_surface = IMG_Load(fileLocationInResources);
    if (newGame->window_surface == NULL)
    {
        printf("Failed to load surface! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    return SDL_CreateTextureFromSurface(newGame->renderer, newGame->window_surface);
}

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
    SDL_DestroyRenderer(theGame->renderer);
    SDL_DestroyWindow(theGame->window);
    SDL_Quit();
}
