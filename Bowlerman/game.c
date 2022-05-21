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
#include <windows.h>
#include <ShellApi.h>
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
#include "player.h"
#include "menu.h"

#define PUBLIC /* empty */
#define PRIVATE static
#define LENGTH 100
void menu();
void initGame(Game theGame, UDPData *udpData, UDPStruct *udpValues, bool *quitGame);
bool checkEvents(Game theGame, Player p[]);
void collisionDetect(Game theGame, Sounds *s, Player p[]);
void showScoreboard(Game theGame, Player p[]);
void process(Game theGame, Sounds *s, Player p[]);
void checkGameOver(Game theGame, Player p[]);
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
void initGame(Game theGame, UDPData *udpData, UDPStruct *udpValues, bool *quitGame)
{
    
    loadAllTextures(theGame);  // Loading textures from file
    
    initSDLNet();           //inits SDL-net
     
    initGUI(theGame);       //Init GUI
    
    initUDP(udpValues);     //init UDP

    menu(theGame, quitGame, udpValues);     //Menu loop
    //TCPstruct tcpValues = createTCPstruct();     //returns a struct for tcp-init-struct.	
	//initTCP(&tcpValues);            //initiates TCP
	//manageTCP(&tcpValues);          //starts TCP
	//closeTCP(&tcpValues);           //closes TCP



    // Init sounds
    initSounds();
    //Init random seed
    srand(time(NULL));
    
    // get playerID via UDP and saves it in theGame->playerIDLocal
    //getPlayerIDviaUDP(theGame, udpData, udpValues);
    
    //Kollar player-ammount (hur många spelare som är online). Just nu är den satt till att alltid vara 4.
    //checkPlayerAmmount(theGame);

    //initierar spelare
    //initAllPlayers(player);

    //ping UDP server so it gets players IP and port.
    pingUDPserver(theGame, udpData, udpValues);

    SDL_Delay(50); //wait  for UDP server to catch up

    
    
    //initierar väggar
    initAllWalls(theGame);

    //set game-delay to x-miliseconds. You should have lower delay if you have a slower computer
    theGame->delayInMS=10;
}

// handles processes, like keyboard-inputs etc
bool checkEvents(Game theGame, Player player[])
{
    // Manag. movement inputs 
    int id = theGame->playerIDLocal;
    manageMovementInputs(theGame, player);
    // Enter game loop (SDL_PollEvent)
    bool quitGame = false;
    while (SDL_PollEvent(&theGame->window_event))
    {
        SDL_Event event = theGame->window_event;
        switch (event.type)
        {
        case SDL_QUIT:
            destroyGame(theGame);
            break;
        case SDL_WINDOWEVENT_CLOSE:
            if (theGame->window)
            {
                destroyGame(theGame);
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
                quitGame = true;
                break;
            case SDLK_t:
                //Testing
                playerIncreaseSpeed(player, id);
                printf("Player speed is now: %lf\n", getPlayerSpeed(player, id));
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
                initGame(theGame, &udpData, &udpValues, &quitGame);         // initializes startvalues. coordinates etc.
                Sounds sounds = initSoundFiles();
                break;
            default:
                break;
            }
        default:
            break;
        }
    }
    return quitGame;
}

// game loop
PUBLIC void gameUpdate(Game theGame)
{
    // Initialize
    bool quitGame = false;
    //Player player[MAXPLAYERS]; // declares x-amounts of players ----- Vad gör ens denna? Players blir väl definierade när vi skapar theGame genom att de ligger i strukten.
    UDPStruct udpValues = createUDPstruct();     //returns a struct for udp-init-struct. Like IP-adress etc.
    UDPData udpData = UDPDataReset();    //Resets data struct, Like player x,y -positions etc.
    initGame(theGame, &udpData, &udpValues, &quitGame);         // initializes startvalues. coordinates etc.
    Sounds sounds = initSoundFiles();
   
    Player player[MAXPLAYERS];
    player[0] = initPlayer(LEFT_X, TOP_Y, 0); // Denna ska snyggas till
    player[1] = initPlayer(RIGHT_X, TOP_Y, 1);
    player[2] = initPlayer(LEFT_X, BOTTOM_Y, 2);
    player[3] = initPlayer(RIGHT_X, BOTTOM_Y, 3);
    // Game Loop:

    while (!quitGame)
    {
        playBackroundMusic(&sounds);
        // Check for events
    
        // Process events (time based stuff)
        process(theGame, &sounds, player);

        quitGame = checkEvents(theGame, player);

        // Collisiondetection
        collisionDetect(theGame, &sounds, player);
        checkGameOver(theGame, player);
        // Send/receive data to server
        manageUDP(theGame, &udpData, &udpValues, player);

        // Update GUI labels (only updates when updateFlag = true)
        updateGUI(theGame, player); //behövs göras om, mem leak (mem leak löst med flagga temporärt)

        // render display
        renderTextures(theGame, player);

        SDL_Delay(theGame->delayInMS); // man behöver ta minus här för att räkna in hur lång tid spelet tar att exekvera
        
        checkGameOver(theGame, player); //Behöver ligga här för att score ska uppdateras innan man dör, NACKDEL att den kollar en extra sak hela tiden.
    }
    destroySoundFiles(sounds);
}

void checkGameOver(Game theGame, Player player[])
{
    static int totallyDeadPlayers = 0;
    for(int i = 0; i < theGame->playerAmount; i++){
        if(playerGetNoOfLives(player, i) == 0){
            if(++totallyDeadPlayers == theGame->playerAmount-1){    //If every player except one is dead, show the scoreboard
                showScoreboard(theGame, player);
            }
        }
    }
}

void showScoreboard(Game theGame, Player player[]) //Måste skriva om den här snyggare
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
    highestScore = playerGetScore(player, id);
    for(int i = 0; i < theGame->playerAmount; i++){
        if(playerGetScore(player, i) > highestScore){    //Basic max-value sorting
            highestScore = playerGetScore(player, i);
            highestScoreID = i;
        }
    }
    printf("Highest score is from player %d\n", highestScoreID+1);
    char buf1[1024];
    char tmpstr1[LEN] = "Player ";
    sprintf(buf1, "%d", highestScoreID+1);
    strcat(tmpstr1, strcat(buf1, " wins!"));

    createLabel(theGame, 0, tmpstr1, -1, black);
    char tmpstr2[LEN] = "SCOREBOARD"; 
    createLabel(theGame, 1, tmpstr2, -1, black);
    char tmpstr3[LEN] = "-------------------"; 
    createLabel(theGame, 2, tmpstr3, -1, black);
    char tmpstr4[LEN] = "Player 1: "; 
    createLabel(theGame, 3, tmpstr4, playerGetScore(player, 0), black);
    char tmpstr5[LEN] = "Player 2: "; 
    createLabel(theGame, 4, tmpstr5, playerGetScore(player, 1), black);
    char tmpstr6[LEN] = "Player 3: "; 
    createLabel(theGame, 5, tmpstr6, playerGetScore(player, 2), black);
    char tmpstr7[LEN] = "Player 4: "; 
    createLabel(theGame, 6, tmpstr7, playerGetScore(player, 3), black);

    for(int i = 0; i < 7; i++){
        SDL_Rect textRect = {WIDTH/3, i*100, width, height};
        SDL_RenderCopy(theGame->renderer, theGame->labels[i], NULL, &textRect);
    }
    SDL_RenderPresent(theGame->renderer); // present renderer
    
    while (loop)
    {
        while(SDL_PollEvent(&theGame->window_event))
        {
            SDL_Event event = theGame->window_event;
            switch(event.type)
            {
                case SDL_QUIT:
                    loop = false;
                    break;
                case SDL_WINDOWEVENT_CLOSE:
                    if(theGame->window)
                    {
                        theGame->window = NULL;
                        loop = false;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_3:
                            printf("\nQUIT SCOREBOARD\n");      //QUIT TO MENU AND RESET GAME VARIABLES IN THE FUTURE?
                            loop = false;
                            break;
                    }
            }  
        }
        SDL_Delay(10);
    }
    for(int i = 0; i < 7; i++){
        SDL_DestroyTexture(theGame->labels[i]);
    }
}

//som en game loop för bomber, kollar timer för explosioner samt bomber
void process(Game theGame, Sounds *s, Player *player)
{
    //kollar bombernas timer, är den klar försvinner bomben och explosionstimer initieras
    for (int i = 0; i < MAXBOMBAMOUNT; i++){
        if (BombGetTimerInit(theGame->bombs[i]) == 1){
                  
            if (BombGetTimerValue(theGame->bombs[i]) == 1){
                initExplosionPosition(theGame, i, player);
                initbowlingballtimer(SDL_GetTicks(), 1000, i);
                testPossibilityToExplodeWithBombs(theGame, i);
                BombSetTimerInit(&theGame->bombs[i], 0);
                BombSetExplosionInit(&theGame->bombs[i], 0);
                BombSetStartvaluetimerbomb(&theGame->bombs[i], SDL_GetTicks());
                playBomb(s);
            }
            else {
                BombSetTimerValue(&theGame->bombs[i], initbowlingballtimer(0, 3000, i));
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
    destroyGUI(theGame);
    SDLNet_Quit();
    SDL_DestroyRenderer(theGame->renderer);
    SDL_DestroyWindow(theGame->window);
    SDL_Quit();
}
void updateScoreFlag(Game theGame, bool cond)
{
    theGame->updateFlag = cond;
}
void setLocalID(Game theGame, int id)
{
    theGame->playerIDLocal = id;
}