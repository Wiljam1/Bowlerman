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
#include "collisionDetection.h"
#include "bomb.h"
#include "textures.h"
#include "wall.h"
#include "udpClient.h"
#include "tcpClient.h"
#include "powerup.h"
#include "sounds.h"
#include "gui.h"
#include "player.h"
#include "menu.h"
// #include <windows.h>
// #include <ShellApi.h> libraries used for ShellExecute();

#define LENGTH 100

/*--- Prototypes in .c file to avoid circular inclusion ---*/
//Initialise game values
PRIVATE void initGame(Game theGame, UDPData *udpData, UDPStruct *udpValues, bool *quitGame, Player *player);
//Process time-based events 
PRIVATE void process(Game theGame, Sounds s, Player p[]);                                                   
//Check if something happened (input etc) 
PRIVATE void checkEvents(Game theGame, Player p[], bool *quitGame);                                         
//Check if every player except one is dead.
PRIVATE void checkGameOver(Game theGame, Player p[], bool *quitGame);                                        
//Check if every player except one is dead.
PRIVATE void showScoreboard(Game theGame, Player p[], bool *quitGame);                                       

// initialises game-window
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
    theGame->playerAmount = 1;                                          // Sets players to minumum amount
    theGame->powerupsNotSent = 0;                                       // Initierar antal powerups att skicka
    return theGame;
}

PRIVATE void initGame(Game theGame, UDPData *udpData, UDPStruct *udpValues, bool *quitGame, Player *player)
{   
     // Loading textures from file
    loadAllTextures(theGame); 

    initSDLNet();          
    
    initGUI(theGame);   
    
    //Menu loop
    menu(theGame, quitGame, udpValues);     
    if (*quitGame == true) return;

    initUDP(udpValues);    

    initAllPlayers(theGame, player);

    //Init random seed
    srand(time(NULL));

    //ping UDP server to update player positions at the start of the game
    pingUDPserver(theGame, udpData, udpValues);

    SDL_Delay(20); //wait  for UDP server to catch up
    
    //initierar väggar
    initAllWalls(theGame);

    //set game-delay to x-miliseconds. You should have lower delay if you have a slower computer
    theGame->delayInMS=10;
    //^^^^^^^^^^^^^^skriv om till funktion
}

PRIVATE void checkEvents(Game theGame, Player player[], bool *quitGame)
{
    // Manag. movement inputs
    int id = getLocalID(theGame);
    manageMovementInputs(theGame, player);
    // Enter game loop (SDL_PollEvent)
    //bool quitGame = false;
    while (SDL_PollEvent(&theGame->window_event))
    {
        SDL_Event event = theGame->window_event;
        switch (event.type)
        {
        case SDL_QUIT:
            *quitGame = true;
            break;
        case SDL_WINDOWEVENT_CLOSE:
            if (theGame->window)
            {
                //destroyGame(theGame);
                *quitGame = true;
            }
            break;
        case SDL_KEYDOWN:
            //if(playerGetIsDead(player, id) == true)                 //NOT ALLOWED TO DO ANY INPUTS IF DEAD
                //break;
            switch (event.key.keysym.sym)
            {
            case SDLK_SPACE:
                tryToPlaceBomb(theGame, theGame->playerIDLocal, player);
                break;
            case SDLK_ESCAPE:
                *quitGame = true;
                break;
            case SDLK_t:
                //Testing
                playerIncreaseSpeed(player, id);
                updateScoreFlag(theGame, true);
                break;
            case SDLK_y:
                //Testing
                playerAddExplosionPower(player, id, 1);
                updateScoreFlag(theGame, true);
                break;
            case SDLK_u:
                //Testing
                playerAddAmountOfBombs(player, id, 1);
                updateScoreFlag(theGame, true);
                break;
            case SDLK_i:
                playerAddLives(player, id, 1);
                playerSetAlive(player, id);
                playerSetInvulnerability(player, id, false);
                updateScoreFlag(theGame, true);
                break;
            case SDLK_p:                                            /*!!! P = RESET-BUTTON!!! (only works when testing alone I think)*/
                //Testing
                quitGame = false;
                //Player player[MAXPLAYERS]; // declares x-amounts of players
                UDPStruct udpValues = createUDPstruct();     //returns a struct for udp-init-struct. Like IP-adress etc.
                UDPData udpData = UDPDataReset();    //Resets data struct, Like player x,y -positions etc.
                initGame(theGame, &udpData, &udpValues, quitGame, player);         // initializes startvalues. coordinates etc.
                Sounds sounds = initSoundFiles();
                break;
            case SDLK_m:
                toggleMusic();
            default:
                break;
            }
        default:
            break;
        }
    }
    //return quitGame;
}

void gameUpdate(Game theGame)
{
    // Initialize
    bool quitGame = false;
    UDPStruct udpValues = createUDPstruct();     //returns a struct for udp-init-struct.
    UDPData udpData = UDPDataReset();            //Resets data struct, Like player x,y -positions etc.

    /*Init all players*/
    Player player[MAXPLAYERS];                  
    initGame(theGame, &udpData, &udpValues, &quitGame, player);         // initialises startvalues. coordinates etc.

    Sounds sounds = initSoundFiles();
    
    // Game Loop:
    while (!quitGame)
    {
        // start background music
        playBackroundMusic(sounds);
    
        // Process events (time based events)
        process(theGame, sounds, player);

        // Check for events (input from player)
        checkEvents(theGame, player, &quitGame);

        // Collisiondetection
        collisionDetect(theGame, sounds, player);

        // Update GUI labels (only updates when updateFlag = true)
        updateGUI(theGame, player);

        // Send/receive data to server
        manageUDP(theGame, &udpData, &udpValues, player);

        // Render display
        renderTextures(theGame, player);

        checkGameOver(theGame, player, &quitGame);

        // Delay, use SDL_GetTicks in the future
        SDL_Delay(theGame->delayInMS);
    }
    destroySoundFiles(sounds);
    free(player);
}

PRIVATE void checkGameOver(Game theGame, Player player[], bool *quitGame)
{
    int totallyDeadPlayers = 0;
    for(int i = 0; i < playerGetPlayerCount(player, 0); i++){  //USING CONSTANT BECAUSE OF BUG
        if(playerGetNoOfLives(player, i) <= 0){ 
            totallyDeadPlayers++;
        }
    }
    if(totallyDeadPlayers == playerGetPlayerCount(player, 0) -1){ //USING CONSTANT BECAUSE OF BUG
        showScoreboard(theGame, player, quitGame);
    }
}

PRIVATE void showScoreboard(Game theGame, Player player[], bool *quitGame)
{
    int x = WIDTH / 2;
    int width = WIDTH / 3;
    int height = WIDTH / 11.7;
    int y1 = HEIGHT/7, y2 = HEIGHT / 6, y3 = HEIGHT / 5, y4 = HEIGHT / 4, y5 = HEIGHT / 3, y6 = HEIGHT / 2, y7 = HEIGHT;
    int id = getLocalID(theGame);
    bool loop = true;
    int status;
    SDL_Color black = {0, 0, 0, 0};
    SDL_Rect backRect = {0, 0, WIDTH, HEIGHT};
    SDL_RenderCopy(theGame->renderer, theGame->background, NULL, &backRect);
    SDL_Texture *scoreLabels[7];
    
    //Determine player with most score
    static int highestScore = 0, highestScoreID = 0;
    for(int i = 0; i < PLAYERAMOUNT; i++){  
        if(playerGetScore(player, i) > highestScore){    //Basic max-value sorting
            highestScore = playerGetScore(player, i);
            highestScoreID = i;
        }
    }
    printf("Highest score is from player %d\n", highestScoreID+1);
    char buf1[LEN];
    char tmpstr1[LEN] = "Player ";
    sprintf(buf1, "%d", highestScoreID+1);
    strcat(tmpstr1, strcat(buf1, " wins!"));

    createLabel(theGame, 0, tmpstr1, -1, black);
    char tmpstr2[LEN] = "SCOREBOARD"; 
    createLabel(theGame, 1, tmpstr2, -1, black);
    char tmpstr3[LEN] = "[3] to restart!"; 
    createLabel(theGame, 2, tmpstr3, -1, black);
    char tmpstr4[LEN] = "Player 1: "; 
    createLabel(theGame, 3, tmpstr4, playerGetScore(player, 0), black);
    char tmpstr5[LEN] = "Player 2: "; 
    createLabel(theGame, 4, tmpstr5, playerGetScore(player, 1), black);
    char tmpstr6[LEN] = "Player 3: "; 
    createLabel(theGame, 5, tmpstr6, playerGetScore(player, 2), black);
    char tmpstr7[LEN] = "Player 4: "; 
    createLabel(theGame, 6, tmpstr7, playerGetScore(player, 3), black);

    for(int i = 0; i < 7; i++){                                                  //Render every label
        SDL_Rect textRect = {WIDTH/3, i*100, width, height};
        SDL_RenderCopy(theGame->renderer, theGame->labels[i], NULL, &textRect);
    }
    SDL_RenderPresent(theGame->renderer);
    
    //Wait for exit or reset
    while (loop)
    {
        while(SDL_PollEvent(&theGame->window_event))
        {
            SDL_Event event = theGame->window_event;
            switch(event.type)
            {
                case SDL_QUIT:
                    *quitGame = true;
                    loop = false;
                    break;
                case SDL_WINDOWEVENT_CLOSE:
                    if(theGame->window)
                    {
                        loop = false;
                        *quitGame = true;
                        theGame->window = NULL;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_3:
                            printf("\nQUIT SCOREBOARD\n");      //QUIT TO MENU AND RESET GAME VARIABLES
                            initAllPlayers(theGame, player);
                            initAllWalls(theGame);
                            loop = false;
                            break;
                        case SDLK_m:
                            toggleMusic();
                        break;
                    }
            }  
        }
        SDL_Delay(50);
    }
    for(int i = 0; i < 7; i++){
        SDL_DestroyTexture(theGame->labels[i]);
    }
}

PRIVATE void process(Game theGame, Sounds s, Player *player)
{
    //Check bomb timer for every bomb
    for (int i = 0; i < MAXBOMBAMOUNT; i++){
        if (BombGetTimerInit(theGame->bombs[i]) == 1){       //Check if bomb is on the field
            if (BombGetTimerValue(theGame->bombs[i]) == 1){  //Check if bomb is supposed to explode
                //Bomb explosion is created
                initExplosionPosition(theGame, i, player);   //SDL_Rect for explosion position
                initbowlingballtimer(SDL_GetTicks(), 1000, i); //How long explosion should be displayed
                testPossibilityToExplodeWithBombs(theGame, i); // Check if explosion is exploding other bombs
                BombSetTimerInit(&theGame->bombs[i], 0);       // Reset timer value
                BombSetExplosionInit(&theGame->bombs[i], 0);   // Init explosion-checking
                BombSetStartvaluetimerbomb(&theGame->bombs[i], SDL_GetTicks()); //Set timer start value
                playBomb(s);                                   // Play bomb sound
            }
            else {
                BombSetTimerValue(&theGame->bombs[i], initbowlingballtimer(0, 3000, i)); // Check if timer is done, then change it
            }
             
        }
    }
    //kollar explosionstimern
    int returnarray[20]={0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3};

    static int currentPowerup = 0;    //Kanske vill göra på ett annat sätt här
    if(currentPowerup == 0)
    {
        currentPowerup = theGame->playerIDLocal;
    }

    for (int i = 0; i < MAXBOMBAMOUNT; i++){
        if (BombGetExplosionInit(theGame->bombs[i]) == 0){
            BombSetExplosionInit(&theGame->bombs[i], initbowlingballtimer(0, 1000, i));
            for(int j=139;j<250;j++){
                if(WallGetDestroyedWall(theGame->wall[j]) == 0){
                    WallSetDestroyedWall(&theGame->wall[j], testCollisionWithDestroyableWalls(theGame, player, j, i));
                    if(WallGetDestroyedWall(theGame->wall[j])){ //If wall is destroyed...
                        if(returnarray[i] == theGame->playerIDLocal){
                            playerAddScore(player, theGame->bombs[i].whoPlacedID, 1);
                            theGame->updateFlag = true;
                            theGame->powerups[currentPowerup] = rollForPowerup(&currentPowerup, currentPowerup, theGame->wall[j].x, theGame->wall[j].y);
                            theGame->powerupsNotSent++;       
                        }
                    }
                }
            }
            if (BombGetExplosionInit(theGame->bombs[i]) == 1){
                BombSetIsPlaced(&theGame->bombs[i], 0);
                if(playerGetAmountOfBombsPlaced(player, returnarray[i]) > 0)
                {
                    playerAddAmountOfBombsPlaced(player, returnarray[i], -1);          
                }
                sortBombsArray(theGame, returnarray[i], player);
                
            }
        }
    }
}

int destroyGame(Game theGame)
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
    destroyGUI(theGame);
    SDLNet_Quit();
    SDL_DestroyRenderer(theGame->renderer);
    SDL_DestroyWindow(theGame->window);
    free(theGame);
    SDL_Quit();
    return 0;
}

void updateScoreFlag(Game theGame, bool cond)
{
    theGame->updateFlag = cond;
}

void setLocalID(Game theGame, int id)
{
    theGame->playerIDLocal = id;
}