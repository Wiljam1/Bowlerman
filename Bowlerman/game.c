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

int playerID=0;        //the players ID. Move eventually

PRIVATE void loadBomb(Game theGame);
PRIVATE void LoadPlayerTextures(Game theGame, int ID, char sourceText[40]);

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
PRIVATE void LoadPlayerTextures(Game theGame, int ID, char sourceText[40])
{
    char txt[40];
    strcpy(txt, sourceText);
    for (int i = 0; i < 12; i++)
    {
        theGame->player_texture[ID][i] = (SDL_Texture *)loadTextures(theGame, txt);
        printf("%s\n", txt);
        txt[26]++;
        if (txt[26] == '9')
        {
            txt[25] = '1';
            txt[26] = '0';
        }
    }
    SDL_FreeSurface(theGame->window_surface);
}

//initializes startvalues for game
void initGame(Game theGame)
{
    //loads in textures
    theGame->background = (SDL_Texture *) loadTextures(theGame, "alley.png");
    theGame->player_texture[0][0] = (SDL_Texture *) loadTextures(theGame, "bowlermantestskins/bowman00.png");
    theGame->player_texture[1][0] = (SDL_Texture *) loadTextures(theGame, "pin2.png");
    theGame->player_texture[2][0] = (SDL_Texture *) loadTextures(theGame, "pin2.png");
    theGame->player_texture[3][0] = (SDL_Texture *) loadTextures(theGame, "pin2.png");
    theGame->bomb_texture = (SDL_Texture *) loadTextures(theGame, "Bowling_Ball_BLue.png");
    theGame->textureWall = (SDL_Texture *) loadTextures(theGame, "wall.png");
    SDL_FreeSurface(theGame->window_surface);

    //check server what ID you have.
    //getPlayerID();
    theGame->playerID=0;

    //detta ska ändras via servern sen.
    theGame->playerAmmount=4;

    //inits x-amount of players
    theGame->player[0] = initPlayer(5, 5);   //sets x and y coordinates and resets values.
    //initPlayerRect(theGame); //inits playerRect[0] to position of player0
    
    if(theGame->playerAmmount>1){
        theGame->player[1] = initPlayer(750, 300);   //sets x and y coordinates and resets values.
    }
    if(theGame->playerAmmount>2){
        theGame->player[2] = initPlayer(0, 300);   //sets x and y coordinates and resets values.
    }
    if(theGame->playerAmmount>3){
        theGame->player[3] = initPlayer(750, 0);   //sets x and y coordinates and resets values.
    }
  
    // //get and scale the dimensions of texture (based on how many players are online)
    // for(int i=0; i<theGame->playerAmmount; i++)
    // {
    //     SDL_QueryTexture(theGame->player_texture[i], NULL, NULL, &playerRect[i].w, &playerRect[i].h);
    //     playerRect[i].w /=7;              //scales down width by 4
    //     playerRect[i].h /=7;              //scales down height by 4  
    // }

    //Init walls / map
    for(int i = 0; i < WALLAMOUNT; i++){
        theGame->wall[i] = initWalls(WALLAMOUNT, 64, 64);
        theGame->wall[i] = wallPlace(i*64, i*32);
    }

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
                switch (event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        done = true; 
                    break;
                    case SDLK_SPACE: 
                    //theGame->bombs[playerID] = initBomb(playerID);
                    //theGame->bombs[playerID].possition.y = theGame->player[playerID].yPos;
                    //theGame->bombs[playerID].possition.x = theGame->player[playerID].xPos;
                    //loadBomb();
                    break;
                    default: break;
                }
                break;
                switch (event.key.keysym.scancode){
                    case SDL_SCANCODE_SPACE: 
                        theGame->possition_ball.y = theGame->player[0].yPos;
                        theGame->possition_ball.x = theGame->player[0].xPos;
                        theGame->possition_ball.h = 50;
                        theGame->possition_ball.w = 50;
                        loadBomb(theGame);
                    break;
                    default: break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.scancode)
                {
                
                    // case SDLK_w: case SDLK_UP:
                    //     up = false;
                    default: break;
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
    // Man kollar vilken direction gubben kollar åt och bestämmer sedan att ett knapptryck kan ta över om det är en ny direction.
    //static int currentDirection = 0;

    int velX = 0, velY = 0;
    
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if(state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_S]){
        velX = -theGame->player[theGame->playerID].speed;
    }
    else if(state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_S]){
        velX = theGame->player[theGame->playerID].speed;
    }
    if(velX == 0){
        if(state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_S]){
            velY = -theGame->player[theGame->playerID].speed;
        }
        else if(state[SDL_SCANCODE_S] && !state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_D]){
            velY = theGame->player[theGame->playerID].speed;
        }
    }


    updatePlayerPos(theGame, velX, velY);
}

void updatePlayerPos(Game theGame, int velX, int velY)
{
    // update (client-side) player positions
    theGame->player[theGame->playerID].xPos += velX;
    theGame->player[theGame->playerID].yPos += velY;
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
    SDL_RenderClear(theGame->renderer); //clear renderer
    //updates/renders background
    SDL_RenderCopy(theGame->renderer, theGame->background, NULL, NULL);
    //render bombs ***********KRASCHAR****************
    //SDL_RenderCopy(theGame->renderer, theGame->bomb_texture[playerID], &bowlingballAnimation[ 0 ], &theGame->bombs[playerID].possition);

    //bubble-sort the players y-position into the array "renderOrder"
    //arraySorter(player, theGame->playerAmmount, renderOrder);


    // renders players
    for(int i=0; i<theGame->playerAmmount; i++){
        SDL_Rect rect = {theGame->player[i].xPos, theGame->player[i].yPos, theGame->player->width, theGame->player->height};
        SDL_RenderCopyEx(theGame->renderer, theGame->player_texture[i][0], 0, &rect, 0, NULL, 0);
    }
    
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



void loadBomb(Game theGame)
{
    bowlingballAnimation[ 0 ].x =   0;
    bowlingballAnimation[ 0 ].y =   0;
    bowlingballAnimation[ 0 ].w = 256;
    bowlingballAnimation[ 0 ].h = 256;

    bowlingballAnimation[ 1 ].x = 256;
    bowlingballAnimation[ 1 ].y =   0;
    bowlingballAnimation[ 1 ].w = 256;
    bowlingballAnimation[ 1 ].h = 256;

    bowlingballAnimation[ 2 ].x = 512;
    bowlingballAnimation[ 2 ].y =   0;
    bowlingballAnimation[ 2 ].w = 256;
    bowlingballAnimation[ 2 ].h = 256;

    bowlingballAnimation[ 3 ].x = 768;
    bowlingballAnimation[ 3 ].y =   0;
    bowlingballAnimation[ 3 ].w = 256;
    bowlingballAnimation[ 3 ].h = 256;

    bowlingballAnimation[ 4 ].x =   0;
    bowlingballAnimation[ 4 ].y = 256;
    bowlingballAnimation[ 4 ].w = 256;
    bowlingballAnimation[ 4 ].h = 256;

    bowlingballAnimation[ 5 ].x = 256;
    bowlingballAnimation[ 5 ].y = 256;
    bowlingballAnimation[ 5 ].w = 256;
    bowlingballAnimation[ 5 ].h = 256;

    bowlingballAnimation[ 6 ].x = 512;
    bowlingballAnimation[ 6 ].y = 256;
    bowlingballAnimation[ 6 ].w = 256;
    bowlingballAnimation[ 6 ].h = 256;

    bowlingballAnimation[ 7 ].x = 768;
    bowlingballAnimation[ 7 ].y = 256;
    bowlingballAnimation[ 7 ].w = 256;
    bowlingballAnimation[ 7 ].h = 256;

    bowlingballAnimation[ 8 ].x =   0;
    bowlingballAnimation[ 8 ].y = 512;
    bowlingballAnimation[ 8 ].w = 256;
    bowlingballAnimation[ 8 ].h = 256;

    bowlingballAnimation[ 9 ].x = 256;
    bowlingballAnimation[ 9 ].y = 512;
    bowlingballAnimation[ 9 ].w = 256;
    bowlingballAnimation[ 9 ].h = 256;

    bowlingballAnimation[ 10 ].x = 512;
    bowlingballAnimation[ 10 ].y = 512;
    bowlingballAnimation[ 10 ].w = 256;
    bowlingballAnimation[ 10 ].h = 256;

    bowlingballAnimation[ 11 ].x = 768;
    bowlingballAnimation[ 11 ].y = 512;
    bowlingballAnimation[ 11 ].w = 256;
    bowlingballAnimation[ 11 ].h = 256;

    bowlingballAnimation[ 12 ].x =   0;
    bowlingballAnimation[ 12 ].y = 768;
    bowlingballAnimation[ 12 ].w = 256;
    bowlingballAnimation[ 12 ].h = 256;

    bowlingballAnimation[ 13 ].x = 256;
    bowlingballAnimation[ 13 ].y = 768;
    bowlingballAnimation[ 13 ].w = 256;
    bowlingballAnimation[ 13 ].h = 256;

    bowlingballAnimation[ 14 ].x = 512;
    bowlingballAnimation[ 14 ].y = 768;
    bowlingballAnimation[ 14 ].w = 256;
    bowlingballAnimation[ 14 ].h = 256;

    bowlingballAnimation[ 15 ].x = 768;
    bowlingballAnimation[ 15 ].y = 768;
    bowlingballAnimation[ 15 ].w = 256;
    bowlingballAnimation[ 15 ].h = 256;

    bowlingballAnimation[ 16 ].x =   0;
    bowlingballAnimation[ 16 ].y =1024;
    bowlingballAnimation[ 16 ].w = 256;
    bowlingballAnimation[ 16 ].h = 256;

    bowlingballAnimation[ 17 ].x = 256;
    bowlingballAnimation[ 17 ].y =1024;
    bowlingballAnimation[ 17 ].w = 256;
    bowlingballAnimation[ 17 ].h = 256;
}

