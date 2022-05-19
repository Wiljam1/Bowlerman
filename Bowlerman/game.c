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

#define PUBLIC /* empty */
#define PRIVATE static
#define LENGTH 100
void menu();
void initGame(Game theGame, UDPData *udpData, UDPStruct *udpValues, bool *quitGame);

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
    initAllPlayers(theGame);
    
    //initierar väggar
    initAllWalls(theGame);

    //set game-delay to x-miliseconds. You should have lower delay if you have a slower computer
    theGame->delayInMS=10;
}

// handles processes, like keyboard-inputs etc
bool checkEvents(Game theGame)
{
    // Manage movement inputs
    manageMovementInputs(theGame);
    
    // Enter game loop (SDL_PollEvent)
    bool quitGame = false;
    while (SDL_PollEvent(&theGame->window_event))
    {
        SDL_Event event = theGame->window_event;
        switch (event.type)
        {
        case SDL_QUIT:
            quitGame = true;
            break;
        case SDL_WINDOWEVENT_CLOSE:
            if (theGame->window)
            {
                theGame->window = NULL;
                quitGame = true;
            }
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_SPACE:
                tryToPlaceBomb(theGame, theGame->playerIDLocal);
                break;
            case SDLK_ESCAPE:
                quitGame = true;
                break;
            case SDLK_t:
                //Testing
                playerIncreaseSpeed(&theGame->player[theGame->playerIDLocal]);
                printf("Player speed is now: %lf\n", getPlayerSpeed(theGame->player[theGame->playerIDLocal]));
                theGame->updateFlag = true;
                break;
            case SDLK_y:
                //Testing
                playerAddExplosionPower(&theGame->player[theGame->playerIDLocal], 1);
                theGame->updateFlag = true;
                break;
            case SDLK_u:
                //Testing
                playerAddAmountOfBombs(&theGame->player[theGame->playerIDLocal], 1);
                theGame->updateFlag = true;
                break;
            case SDLK_i:
                playerAddLives(&theGame->player[theGame->playerIDLocal], 1);
                theGame->updateFlag = true;
                break;
            case SDLK_p:                                            /*!!! P = RESET-BUTTON!!! (only works when testing alone I think)*/
                //Testing
                quitGame = false;
                Player player[MAXPLAYERS]; // declares x-amounts of players
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

void manageMovementInputs(Game theGame)
{
    double velX = 0, velY = 0;
    int id = getLocalID(theGame);
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
    playerSetMoveDirection(&theGame->player[id], direction);
    updatePlayerXPosition(&theGame->player[id], velX);
    updatePlayerYPosition(&theGame->player[id], velY);
}

// game loop
PUBLIC void gameUpdate(Game theGame)
{
    // Initialize
    bool quitGame = false;
    Player player[MAXPLAYERS]; // declares x-amounts of players
    UDPStruct udpValues = createUDPstruct();     //returns a struct for udp-init-struct. Like IP-adress etc.
    UDPData udpData = UDPDataReset();    //Resets data struct, Like player x,y -positions etc.
    initGame(theGame, &udpData, &udpValues, &quitGame);         // initializes startvalues. coordinates etc.
    Sounds sounds = initSoundFiles();
    // Game Loop:

    while (!quitGame)
    {
        //playBackroundMusic(&sounds);
        // Check for events
    
        // Process events (time based stuff)
        process(theGame, &sounds);

        quitGame = checkEvents(theGame);

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

void checkGameOver(Game theGame)
{
    static int totallyDeadPlayers = 0;
    for(int i = 0; i < PLAYERAMOUNT; i++){
        if(playerGetNoOfLives(theGame->player[i]) == 0){
            if(++totallyDeadPlayers == 1){
                showScoreboard(theGame);
            }
        }
    }
}

void showScoreboard(Game theGame) //Måste skriva om den här snyggare
{
    int x = WIDTH / 2;
    int width = WIDTH / 3;
    int height = WIDTH / 11.7;
    int y1 = HEIGHT/7, y2 = HEIGHT / 6, y3 = HEIGHT / 5, y4 = HEIGHT / 4, y5 = HEIGHT / 3, y6 = HEIGHT / 2, y7 = HEIGHT;

    bool loop = true;
    int status;
    SDL_Color black = {0, 0, 0, 0};
    SDL_Rect backRect = {0, 0, WIDTH, HEIGHT};
    SDL_RenderCopy(theGame->renderer, theGame->background, NULL, &backRect);
    
    SDL_Texture *scoreLabels[7];
    char buf[1024];

    theGame->window_surface = TTF_RenderText_Blended(theGame->font, "GAME OVER!", black);
    scoreLabels[0] = SDL_CreateTextureFromSurface(theGame->renderer, theGame->window_surface);
    theGame->window_surface = TTF_RenderText_Blended(theGame->font, "SCOREBOARD", black);
    scoreLabels[1] = SDL_CreateTextureFromSurface(theGame->renderer, theGame->window_surface);
    theGame->window_surface = TTF_RenderText_Blended(theGame->font, "-----------", black);
    scoreLabels[2] = SDL_CreateTextureFromSurface(theGame->renderer, theGame->window_surface);
    strcpy(buf, "Player 1: ");
    char num[30];
        sprintf(num, "%d", theGame->player[0].score);  //Convert float to string
        strcat(buf, num);          //Add float to end of string
    theGame->window_surface = TTF_RenderText_Blended(theGame->font, buf, black);
    scoreLabels[3] = SDL_CreateTextureFromSurface(theGame->renderer, theGame->window_surface);
    strcpy(buf, "Player 2: ");
        sprintf(num, "%d", theGame->player[1].score);  //Convert float to string
        strcat(buf, num);          //Add float to end of string
    theGame->window_surface = TTF_RenderText_Blended(theGame->font, buf, black);
    scoreLabels[4] = SDL_CreateTextureFromSurface(theGame->renderer, theGame->window_surface);
    strcpy(buf, "Player 3: ");
        sprintf(num, "%d", theGame->player[2].score);  //Convert float to string
        strcat(buf, num);          //Add float to end of string
    theGame->window_surface = TTF_RenderText_Blended(theGame->font, buf, black);
    scoreLabels[5] = SDL_CreateTextureFromSurface(theGame->renderer, theGame->window_surface);
    strcpy(buf, "Player 4: ");
        sprintf(num, "%d", theGame->player[3].score);  //Convert float to string
        strcat(buf, num);          //Add float to end of string
    theGame->window_surface = TTF_RenderText_Blended(theGame->font, buf, black);
    scoreLabels[6] = SDL_CreateTextureFromSurface(theGame->renderer, theGame->window_surface);
    free(num);
    SDL_FreeSurface(theGame->window_surface);
    for(int i = 0; i < 7; i++){
        SDL_Rect textRect = {WIDTH/3, i*100, width, height};
        SDL_RenderCopy(theGame->renderer, scoreLabels[i], NULL, &textRect);
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
                        case SDLK_1:
                            // ip = writeIP(); / fetch user IP
                            // createServer(ip); -Starta eller kompilera om(?) servern med användarens IP
                            // joinLobby(ip);  -Scen där man ser vilka som har joinat lobbyn
                            printf("\nHOST SERVER\n");
                            printf("Creating server... ");
                            ShellExecuteA(GetDesktopWindow(),"open","udpServer.exe",NULL,NULL,SW_SHOW);  //Start server file
                            SDL_Delay(1000);
                            printf("Server created!\n");
                            //*quitGame = false;
                            loop = false;
                            break;
                        case SDLK_2:
                            printf("\nJOIN SERVER\n");
                            //Nån menyfunktion där man skriver in IPadressen för hosten
                            //just nu hårdkodad.
                            // ip = writeIP(); -Scen där man får skriva IP:n man vill joina
                            // joinLobby(ip);  -Scen där man ser vilka som har joinat lobbyn
                            char ip[] = "127.0.0.1";
                            //strcpy(udpvalues->serverIp, ip);
                            //*quitGame = false;
                            loop = false;
                            break;
                        case SDLK_3:
                            printf("\nQUIT GAME\n");
                            //*quitGame = true;
                            loop = false;
                            break;
                        //case: OPTIONS (inte så viktigt)
                        //case: CREDITS (inte så viktigt)
                    }
            }  
        }
        SDL_Delay(10);
    }
    for(int i = 0; i < 7; i++){
        SDL_DestroyTexture(scoreLabels[i]);
    }
}

//som en game loop för bomber, kollar timer för explosioner samt bomber
void process(Game theGame, Sounds *s)
{
    //kollar bombernas timer, är den klar försvinner bomben och explosionstimer initieras
    for (int i = 0; i < MAXBOMBAMOUNT; i++){
        if (BombGetTimerInit(theGame->bombs[i]) == 1){
                  
            if (BombGetTimerValue(theGame->bombs[i]) == 1){
                initExplosionPosition(theGame, i);
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
                    WallSetDestroyedWall(&theGame->wall[j], testCollisionWithDestroyableWalls(theGame, j, i));
                    if(WallGetDestroyedWall(theGame->wall[j])){ //If wall is destroyed...
                        if(returnarray[i] == theGame->playerIDLocal){
                            playerAddScore(&theGame->player[theGame->bombs[i].whoPlacedID], 1);
                            theGame->updateFlag = true;
                            theGame->powerups[currentPowerup] = rollForPowerup(&currentPowerup, currentPowerup, theGame->wall[j].x, theGame->wall[j].y);       
                        }
                    }
                }
            }
            if (BombGetExplosionInit(theGame->bombs[i]) == 1){
                BombSetIsPlaced(&theGame->bombs[i], 0);
                if(playerGetAmountOfBombsPlaced(theGame->player[returnarray[i]]) > 0)
                {
                    playerAddAmountOfBombsPlaced(&theGame->player[returnarray[i]], -1);          
                }
                sortBombsArray(theGame, returnarray[i]);
                
            }
        }
    }
}

void menu(Game theGame, bool *quitGame, UDPStruct *udpvalues)
{
    int x = WIDTH / 5.85;
    int width = WIDTH / 3;
    int height = WIDTH / 11.7;
    int y1 = x; int y2 = WIDTH / 4; int y3 = width;

    
    int status;
    SDL_Color black = {0, 0, 0, 0};
    SDL_Rect backRect = {0, 0, WIDTH, HEIGHT};
    SDL_RenderCopy(theGame->renderer, theGame->background, NULL, &backRect);
    
    theGame->window_surface = TTF_RenderText_Blended(theGame->font, "1) HOST SERVER", black);
    SDL_Texture *menuT1 = SDL_CreateTextureFromSurface(theGame->renderer, theGame->window_surface);
    theGame->window_surface = TTF_RenderText_Blended(theGame->font, "2) JOIN SERVER", black);
    SDL_Texture *menuT2 = SDL_CreateTextureFromSurface(theGame->renderer, theGame->window_surface);
    theGame->window_surface = TTF_RenderText_Blended(theGame->font, "3) QUIT GAME", black);
    SDL_Texture *menuT3 = SDL_CreateTextureFromSurface(theGame->renderer, theGame->window_surface);
    //SDL_FreeSurface(theGame->window_surface);

    SDL_Rect textRect1 = {x, y1, width, height};
    SDL_Rect textRect2 = {x, y2, width, height};
    SDL_Rect textRect3 = {x, y3, width, height}; 
    SDL_RenderCopy(theGame->renderer, menuT1, NULL, &textRect1);
    SDL_RenderCopy(theGame->renderer, menuT2, NULL, &textRect2);
    SDL_RenderCopy(theGame->renderer, menuT3, NULL, &textRect3);

    SDL_RenderPresent(theGame->renderer); // present renderer

    SDL_Thread* thread; //thread used for TCP-waiting (when joining game)
    TCPstruct tcpValues = createTCPstruct();     //returns a struct for tcp-init-struct.	
    ThreadStruct threadStruct;          //creates a struct for threads for TCP
	threadStruct.startflag=0;
    
    int startGame=false;
    bool breakLoop = false;
    while (!breakLoop)
    {
        while(SDL_PollEvent(&theGame->window_event))
        {
            SDL_Event event = theGame->window_event;
            switch(event.type)
            {
                case SDL_QUIT:
                    *quitGame = true;
                    breakLoop = true;
                    break;
                case SDL_WINDOWEVENT_CLOSE:
                    if(theGame->window)
                    {
                        theGame->window = NULL;
                        *quitGame = true;
                        breakLoop = true;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_1: //host server
                            // ip = writeIP(); / fetch user IP
                            // createServer(ip); -Starta eller kompilera om(?) servern med användarens IP
                            // joinLobby(ip);  -Scen där man ser vilka som har joinat lobbyn
                            printf("\nHOST SERVER\n");
                            printf("Creating server... ");
                            ShellExecuteA(GetDesktopWindow(),"open","udpServer.exe",NULL,NULL,SW_SHOW);  //Start server file
                            SDL_Delay(1000);
                            printf("Server created!\n");

                            char ip1[]={"127.0.0.1"};
                            strcpy(udpvalues->serverIp, ip1);
                            initTCP(&tcpValues, ip1);		//connectar till angiven Ip-adress
                            threadStruct.sd=tcpValues.sd;  //copy socket descriptor into thread-struct
                            theGame->playerIDLocal = getPlayerIDviaTCP(&tcpValues); 
                            printf("playerID: %d\n", theGame->playerIDLocal);
                            startGameViaTCP(&tcpValues, &threadStruct); //starts the game, and sends the info out to all other clients
                            *quitGame = false;
                            //breakLoop = true;
                            break;
                        case SDLK_2: //join server
                            printf("\nJOIN SERVER\n");
                            //Nån menyfunktion där man skriver in IPadressen för hosten
                            //just nu hårdkodad.
                            // ip = writeIP(); -Scen där man får skriva IP:n man vill joina
                            // joinLobby(ip);  -Scen där man ser vilka som har joinat lobbyn
                            char ip2[]={"127.0.0.1"};
                            strcpy(udpvalues->serverIp, ip2);
                            
                            initTCP(&tcpValues, ip2);		//connectar till angiven Ip-adress
                            threadStruct.sd=tcpValues.sd; //copy socket descriptor into thread-struct
                            theGame->playerIDLocal = getPlayerIDviaTCP(&tcpValues); 
                            printf("playerID: %d\n", theGame->playerIDLocal);
                            thread = SDL_CreateThread(receiveTCP, "test", (void *) &threadStruct); //creates a thread waiting to see if the game is starting or not.

                            *quitGame = false;
                            //breakLoop = true;
                            break;
                        case SDLK_3:
                            printf("\nQUIT GAME\n");
                            *quitGame = true;
                            breakLoop = true;
                            break;
                        //case: OPTIONS (inte så viktigt)
                        //case: CREDITS (inte så viktigt)
                    }
            }  
        }
        if(threadStruct.startflag==1)
		{
			printf("playerammount: %d\n", threadStruct.playerAmmount);
			printf("startflag: %d\n", threadStruct.startflag);
            theGame->playerAmount=threadStruct.playerAmmount;
			//get playerammount & start game (exit breakLoop)
			//thread=NULL;  //closes thread
            breakLoop=true;
			break;
		}
        SDL_Delay(50);
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
void flagSetUpdate(Game theGame, bool cond)
{
    theGame->updateFlag = cond;
}
void setLocalID(Game theGame, int id)
{
    theGame->playerIDLocal = id;
}