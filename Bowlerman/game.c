#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "collissionDetection.h"
#include "sorter.h"
#include "bomb.h"

#define PUBLIC /* empty */
#define PRIVATE static
#define LENGTH 100

const int WIDTH = 800;  //Move eventually
const int HEIGHT = 450;

int playerID = 0;       //the players ID. Move eventually

//initializes game
PUBLIC Game createWindow()
{
    Game theGame = malloc(sizeof(struct game_type));
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Failed to initialize SDL2: %s\n", SDL_GetError());
    }
    theGame->window= SDL_CreateWindow("Bowlerman 0.1", 
                                       SDL_WINDOWPOS_CENTERED, 
                                       SDL_WINDOWPOS_CENTERED, 
                                       WIDTH, HEIGHT, 
                                       SDL_WINDOW_SHOWN);
    
    theGame->renderer = SDL_CreateRenderer(theGame->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    theGame->window_surface = SDL_GetWindowSurface(theGame->window);
    return theGame;
}

//initializes startvalues for game
void initGame(Game theGame)
{
    //loads in textures
    theGame->background = (SDL_Texture *) loadTextures(theGame, "alley.png");
    theGame->player_texture[0][0] = (SDL_Texture *) loadTextures(theGame, "bowlermantestskins/bowman00.png");
    theGame->player_texture[1][0] = (SDL_Texture *) loadTextures(theGame, "redman/down.png");
    theGame->player_texture[1][1] = (SDL_Texture *) loadTextures(theGame, "redman/right.png");
    theGame->player_texture[1][2] = (SDL_Texture *) loadTextures(theGame, "redman/up.png");
    theGame->player_texture[2][0] = (SDL_Texture *) loadTextures(theGame, "pin2.png");
    theGame->player_texture[3][0] = (SDL_Texture *) loadTextures(theGame, "pin2.png");
    theGame->bomb_texture[0] = (SDL_Texture *) loadTextures(theGame, "Bowling_Ball_BLue.png");
    theGame->bomb_texture[1] = (SDL_Texture *) loadTextures(theGame, "Bowling_Ball_Purple.png");
    theGame->bomb_texture[2] = (SDL_Texture *) loadTextures(theGame, "Bowling_Ball_Red.png");
    theGame->bomb_texture[3] = (SDL_Texture *) loadTextures(theGame, "Bowling_Ball_Yellow.png");
    theGame->textureWall = (SDL_Texture *) loadTextures(theGame, "wall.png");
    SDL_FreeSurface(theGame->window_surface);

    theGame->pSprites = getRedPlayer();

    //check server what ID you have.
    //getPlayerID();
    theGame->playerID=0;

    //detta ska ändras via servern sen.
    theGame->playerAmmount=4;

    //inits x-amount of players
    theGame->player[0] = initPlayer(70, 70);   //sets x and y coordinates and resets values.
    //initPlayerRect(theGame); //inits playerRect[0] to position of player0
    
    if(theGame->playerAmmount>1){
        theGame->player[1] = initPlayer(700, 300);   //sets x and y coordinates and resets values.
    }
    if(theGame->playerAmmount>2){
        theGame->player[2] = initPlayer(70, 300);   //sets x and y coordinates and resets values.
    }
    if(theGame->playerAmmount>3){
        theGame->player[3] = initPlayer(700, 70);   //sets x and y coordinates and resets values.
    }
  
    // //get and scale the dimensions of texture (based on how many players are online)
    // for(int i=0; i<theGame->playerAmmount; i++)
    // {
    //     SDL_QueryTexture(theGame->player_texture[i], NULL, NULL, &playerRect[i].w, &playerRect[i].h);
    //     playerRect[i].w /=7;              //scales down width by 4
    //     playerRect[i].h /=7;              //scales down height by 4  
    // }

    //Init walls / map
    int wallwidth = 48;  //Vet inte hur vi ska bestämma dehär variablerna riktigt,
    int wallheight = 48; // Om de ens kommer användas
    for(int i = 0; i < WALLAMOUNT; i++){
        theGame->wall[i] = initWalls(WALLAMOUNT, wallwidth, wallheight);
        if(i < 20){
            theGame->wall[i] = wallPlace(i*wallwidth, 0);
        }
        else if(i < 40){
            theGame->wall[i] = wallPlace(i*wallwidth-WIDTH, HEIGHT-wallwidth);
        }
        else if(i < 60){
            theGame->wall[i] = wallPlace(0, i*wallwidth-HEIGHT*4);
        }
        else if(i < 80){
            theGame->wall[i] = wallPlace(WIDTH-wallwidth, i*wallwidth-HEIGHT*6);
        }
        else{

        }
    }

}

//handles processes, like keyboard-inputs etc
bool checkEvents(Game theGame)
{
    //Enter game loop (SDL_PollEvent)
    bool done = false;
    while(SDL_PollEvent(&theGame->window_event)){
        SDL_Event event = theGame->window_event;

        switch(event.type)
        {
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
                switch (event.key.keysym.sym)
                {
                    case SDLK_SPACE: 
                        theGame->bombs[playerID] = initBomb(playerID);
                        theGame->bombs[playerID].position.y = getPlayerYPosition(theGame->player[playerID])+16;
                        theGame->bombs[playerID].position.x = getPlayerXPosition(theGame->player[playerID])-5;
                        loadBomb();
                        theGame->bombs[playerID].timervalue = initbowlingballtimer(SDL_GetTicks());
                        theGame->bombs[playerID].timerinit = 1;
                    break;
                    case SDLK_ESCAPE: done = true; 
                        break;
                    case SDLK_w:
                        //theGame->moveDirection[0] = 'w';
                        break;
                    case SDLK_a:
                        //theGame->moveDirection[0] = 'a';
                        break;
                    case SDLK_s:
                        //theGame->moveDirection[0] = 's';
                        break;
                    case SDLK_d:
                        //theGame->moveDirection[0] = 'd';
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
                    case SDLK_w:
                        break;
                    case SDLK_a:
                        break;
                    case SDLK_s:
                        break;
                    case SDLK_d:
                        break;
                    default:
                        theGame->moveDirection[0] = '0';
                        break;
                }
                break;
            default: 
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
    // Man kollar vilken direction gubben kollar åt och bestämmer sedan att ett knapptryck kan ta över om det är en ny direction.
    //static int currentDirection = 0;

    int velX = 0, velY = 0;
    Player player = theGame->player[playerID];
    
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_S]){
        velX = -getPlayerSpeed(player);
        theGame->moveDirection[0] = 'a';
    }
    else if(state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_S]){
        velX = getPlayerSpeed(player);
        theGame->moveDirection[0] = 'd';
    }
    if(velX == 0){
        if(state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_S]){
            velY = -getPlayerSpeed(player);
            theGame->moveDirection[0] = 'w';
        }
        else if(state[SDL_SCANCODE_S] && !state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_D]){
            velY = getPlayerSpeed(player);
            theGame->moveDirection[0] = 's';
        }
    }
    //Update player positions
    theGame->player[playerID].xPos += velX;
    theGame->player[playerID].yPos += velY;
}

//game loop
PUBLIC void gameUpdate(Game theGame) 
{
    Player player[MAXPLAYERS];   //declares x-ammounts of players
    initGame(theGame); //initializes startvalues. coordinates etc.
    //int renderOrder[4]={0,1,2,3}; //what order to render players
    //gameloop:
    bool done = false;
    while(!done)
    {
        //Check for events
        done = checkEvents(theGame);

        //Process events (time based stuff)
        if (theGame->bombs[playerID].timerinit == 1)
        {
            theGame->bombs[playerID].timervalue = initbowlingballtimer(0);
            if(theGame->bombs[playerID].timervalue == 1){theGame->bombs[playerID].timerinit = 0;}
        }
        //process();

        //Collisiondetection
        collisionDetect(theGame);

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
    //clear renderer
    SDL_RenderClear(theGame->renderer);

    //updates/renders background
    SDL_RenderCopy(theGame->renderer, theGame->background, NULL, NULL);

    //render bombs
    if (theGame->bombs[playerID].timervalue == 0)
    {
        SDL_RenderCopy(theGame->renderer, theGame->bomb_texture[playerID], &bowlingballAnimation[ 0 ], &theGame->bombs[playerID].position);
    }

    //bubble-sort the players y-position into the array "renderOrder"
    //arraySorter(player, theGame->playerAmmount, renderOrder);

    //Draw walls
    for (int i = 0; i < WALLAMOUNT; i++)
    {
        SDL_Rect wallRect = {theGame->wall[i].x, theGame->wall[i].y, theGame->wall[i].w, theGame->wall[i].h};
        SDL_RenderCopy(theGame->renderer, theGame->textureWall, NULL, &wallRect);
    }

    //SDL_RenderCopy(theGame->renderer, theGame->bomb_texture[playerID], &bowlingballAnimation[ 0 ], &theGame->bombs[playerID].position);

    // renders player**** EMIL TESTAR HÄR*****
    static int updateSprite = 0;
    static int spriteTimer = 0;
    if (spriteTimer > 10) spriteTimer = 0; // Vi får komma på en bra timing för animationsuppdatering alt. en bättre lösning.
    SDL_Rect playerRect = {theGame->player[playerID].xPos, theGame->player[playerID].yPos, theGame->player->width, theGame->player->height};
    
    if (theGame->moveDirection[playerID] == 'w')
    {
        SDL_RenderCopyEx(theGame->renderer, theGame->player_texture[1][2], &theGame->pSprites.BowlerMan[updateSprite], &playerRect, 0, NULL, 0);
        if (spriteTimer++ % 5 == 0) updateSprite++;
    }
    else if (theGame->moveDirection[playerID] == 'a')
    {
        SDL_RenderCopyEx(theGame->renderer, theGame->player_texture[1][1], &theGame->pSprites.BowlerMan[updateSprite], &playerRect, 0, NULL, 1);
        if (spriteTimer++ % 5 == 0) updateSprite++;
    }
    else if (theGame->moveDirection[playerID] == 's')
    {
        SDL_RenderCopyEx(theGame->renderer, theGame->player_texture[1][0], &theGame->pSprites.BowlerMan[updateSprite], &playerRect, 0, NULL, 0);
        if (spriteTimer++ % 5 == 0) updateSprite++;
    }
    else if (theGame->moveDirection[playerID] == 'd')
    {
        SDL_RenderCopyEx(theGame->renderer, theGame->player_texture[1][1], &theGame->pSprites.BowlerMan[updateSprite], &playerRect, 0, NULL, 0);
        if (spriteTimer++ % 5 == 0) updateSprite++;
    }
    else SDL_RenderCopyEx(theGame->renderer, theGame->player_texture[1][0], &theGame->pSprites.BowlerMan[0], &playerRect, 0, NULL, 0);
    if (updateSprite > 7) updateSprite = 0;

    //SDL_Rect rect1 = {theGame->player[2].xPos, theGame->player[2].yPos, theGame->player->width, theGame->player->height};
    //SDL_RenderCopyEx(theGame->renderer, theGame->player_texture[1][0], &theGame->player->playerRect, &theGame->playerRect[0][0], 0, NULL, 0);

    //SDL_RenderCopy(theGame->renderer, theGame->player_texture[1][0], &theGame->pSprites.redMan[0][0], &theGame->player->playerRect);
    
    //Draw GUI last (top of screenlayers)

    SDL_RenderPresent(theGame->renderer); //present renderer
}

PUBLIC void destroyGame(Game theGame)
{
    SDL_DestroyTexture(theGame->background);
    for (int i = 0; i < 4; i++)
        SDL_DestroyTexture(theGame->player_texture[i][0]);
    SDL_DestroyRenderer(theGame->renderer);
    SDL_DestroyWindow(theGame->window);
    SDL_Quit();
}