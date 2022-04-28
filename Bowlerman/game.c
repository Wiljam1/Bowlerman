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

const int WIDTH = 800; // Move eventually
const int HEIGHT = 450;

void initExplosionPosition(Game theGame);

//int playerID = 0;       //the players ID. Move eventually

//initializes game
PRIVATE void loadAllTextures(Game theGame);
PRIVATE void UpdatePlayerTextures(Game theGame);
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

    theGame->renderer = SDL_CreateRenderer(theGame->window, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC);
    theGame->window_surface = SDL_GetWindowSurface(theGame->window);
    return theGame;
}

// initializes startvalues for game
void initGame(Game theGame)
{
    // Loading textures from file
    loadAllTextures(theGame);
    // check server what ID you have.
    // getPlayerID();
    theGame->playerID = 1;

    // detta ska ändras via servern sen.
    theGame->playerAmmount = 4;

    //allow bomb placement init
    for (int i=0;i<4;i++)
    {
        theGame->allowBombPlacement[i] = 1;
    }
    // inits x-amount of players
    theGame->player[0] = initPlayer(70, 70); // sets x and y coordinates and resets values.
    // initPlayerRect(theGame); //inits playerRect[0] to position of player0

    if (theGame->playerAmmount > 1)
    {
        theGame->player[1] = initPlayer(700, 300); // sets x and y coordinates and resets values.
    }
    if (theGame->playerAmmount > 2)
    {
        theGame->player[2] = initPlayer(70, 300); // sets x and y coordinates and resets values.
    }
    if (theGame->playerAmmount > 3)
    {
        theGame->player[3] = initPlayer(700, 70); // sets x and y coordinates and resets values.
    }

    // //get and scale the dimensions of texture (based on how many players are online)
    // for(int i=0; i<theGame->playerAmmount; i++)
    // {
    //     SDL_QueryTexture(theGame->player_texture[i], NULL, NULL, &playerRect[i].w, &playerRect[i].h);
    //     playerRect[i].w /=7;              //scales down width by 4
    //     playerRect[i].h /=7;              //scales down height by 4
    // }

    // Init walls / map
    int wallwidth = 48;  // Vet inte hur vi ska bestämma dehär variablerna riktigt,
    int wallheight = 48; // Om de ens kommer användas
    for (int i = 0; i < WALLAMOUNT; i++)
    {
        theGame->wall[i] = initWalls(WALLAMOUNT, wallwidth, wallheight);
        if (i < 20)
        {
            theGame->wall[i] = wallPlace(i * wallwidth, 0);
        }
        else if (i < 40)
        {
            theGame->wall[i] = wallPlace(i * wallwidth - WIDTH, HEIGHT - wallwidth);
        }
        else if (i < 60)
        {
            theGame->wall[i] = wallPlace(0, i * wallwidth - HEIGHT * 4);
        }
        else if (i < 80)
        {
            theGame->wall[i] = wallPlace(WIDTH - wallwidth, i * wallwidth - HEIGHT * 6);
        }
        else
        {
        }
    }
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
                if(theGame->allowBombPlacement[theGame->playerID] == 1)
                {
                    theGame->allowBombPlacement[theGame->playerID] = 0;
                    theGame->bombs[theGame->playerID] = initBomb(theGame->playerID);
                    theGame->bombs[theGame->playerID].position.y = getPlayerYPosition(theGame->player[theGame->playerID]) + 16;
                    theGame->bombs[theGame->playerID].position.x = getPlayerXPosition(theGame->player[theGame->playerID]) - 5;
                    theGame->bombs[theGame->playerID].timervalue = initbowlingballtimer(SDL_GetTicks(), 3000);
                    theGame->bombs[theGame->playerID].timerinit = 1;
                }
                break;
            case SDLK_ESCAPE:
                done = true;
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
                // theGame->moveDirection[0] = '0';
                break;
            }
            break;
        default:
            break;
        }
    }

    // Manage movement inputs
    manageMovementInputs(theGame);

    // send and retrive positions via server

    return done;
}

void manageMovementInputs(Game theGame)
{
    // Om du ska fortsätta göra movement bättre;
    // https://stackoverflow.com/questions/39929853/priority-when-2-keys-are-pressed-at-the-same-time-script-for-a-game
    // Man kollar vilken direction gubben kollar åt och bestämmer sedan att ett knapptryck kan ta över om det är en ny direction.
    // static int currentDirection = 0;

    int velX = 0, velY = 0;
    Player player = theGame->player[theGame->playerID];
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_S])
    {
        velX = -getPlayerSpeed(player);
        theGame->player[theGame->playerID].moveDirection = 'a';
    }
    else if (state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_S])
    {
        velX = getPlayerSpeed(player);
        theGame->player[theGame->playerID].moveDirection = 'd';
    }
    if (velX == 0)
    {
        if (state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_S])
        {
            velY = -getPlayerSpeed(player);
            theGame->player[theGame->playerID].moveDirection = 'w';
        }
        else if (state[SDL_SCANCODE_S] && !state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_D])
        {
            velY = getPlayerSpeed(player);
            theGame->player[theGame->playerID].moveDirection = 's';
        }
    }
    if (!velX && !velY)
        theGame->player[theGame->playerID].moveDirection = '0';
    // Update player positions
    theGame->player[theGame->playerID].xPos += velX;
    theGame->player[theGame->playerID].yPos += velY;
}

// game loop
PUBLIC void gameUpdate(Game theGame)
{
    Player player[MAXPLAYERS]; // declares x-ammounts of players
    initGame(theGame);         // initializes startvalues. coordinates etc.
    // int renderOrder[4]={0,1,2,3}; //what order to render players
    // gameloop:
    bool done = false;
    while (!done)
    {
        // Check for events
        done = checkEvents(theGame);

        // Process events (time based stuff)
        if (theGame->bombs[theGame->playerID].timerinit == 1)
        {
            theGame->bombs[theGame->playerID].timervalue = initbowlingballtimer(0, 3000);
            if(theGame->bombs[theGame->playerID].timervalue == 1){
                theGame->bombs[theGame->playerID].timerinit = 0;
                theGame->bombs[theGame->playerID].explosioninit = 0;
                initExplosionPosition(theGame);
                initbowlingballtimer(SDL_GetTicks(), 1000);
                
                }
        }
        if(theGame->bombs[theGame->playerID].explosioninit == 0)
        {
            theGame->bombs[theGame->playerID].explosioninit = initbowlingballtimer(0, 1000);
            if(theGame->bombs[theGame->playerID].explosioninit == 1)
            {
                theGame->allowBombPlacement[theGame->playerID] = 1;
            }
        }
        
        // process();

        // Collisiondetection
        collisionDetect(theGame);

        // Send/receive data to server

        // render display
        renderTextures(theGame);

        SDL_Delay(1000/60); // man behöver ta minus här för att räkna in hur lång tid spelet tar att exekvera
        /* static int a = 0;
        static int b = 0;
        a++;
        if (a == 60)
        {
            printf("%d ", ++b);
            a = 0;
        } */
    }
}

// loads media into texture
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

// renders background and players etc.
void renderTextures(Game theGame)
{
    // Define stuff to make function easier to read
    SDL_Renderer *renderer = theGame->renderer;
    int id = theGame->playerID;

    // clear renderer
    SDL_RenderClear(renderer);

    // updates/renders background
    SDL_RenderCopy(renderer, theGame->background, NULL, NULL);

    // render bombs
    if (theGame->bombs[id].timervalue == 0)
    {
        SDL_RenderCopy(renderer, theGame->bomb_texture[id], &bowlingballAnimation[0], &theGame->bombs[id].position);
    }

    //render bomb explosions
    if (theGame->bombs[theGame->playerID].explosioninit == 0)
    {
        for (int i=0;i < 5;i++)
        {
            SDL_RenderCopy(theGame->renderer, theGame->bombExplosion_texture, &bowlingballAnimation[ 0 ], &theGame->explosionPosition[i]);
        }    
    }

    //bubble-sort the players y-position into the array "renderOrder"
    //arraySorter(player, theGame->playerAmmount, renderOrder);

    //Draw walls
    for (int i = 0; i < WALLAMOUNT; i++)
    {
        SDL_Rect wallRect = {theGame->wall[i].x, theGame->wall[i].y, theGame->wall[i].w, theGame->wall[i].h};
        SDL_RenderCopy(renderer, theGame->textureWall, NULL, &wallRect);
    }

    // Updating textures depending on movement
    UpdatePlayerTextures(theGame);
    // Draw GUI last (top of screenlayers)

    SDL_RenderPresent(renderer); // present renderer
}
PRIVATE void loadAllTextures(Game theGame)
{
    // loads in textures
    theGame->background = (SDL_Texture *)loadTextures(theGame, "alley.png");
    /*PLAYER ID 0*/
    theGame->player_texture[0][0] = (SDL_Texture *)loadTextures(theGame, "redman/down.png");
    theGame->player_texture[0][1] = (SDL_Texture *)loadTextures(theGame, "redman/up.png");
    theGame->player_texture[0][2] = (SDL_Texture *)loadTextures(theGame, "redman/right.png");
    theGame->player_texture[0][3] = (SDL_Texture *)loadTextures(theGame, "redman/left.png");
    /*PLAYER ID 1*/
    theGame->player_texture[1][0] = (SDL_Texture *)loadTextures(theGame, "redman/down.png");
    theGame->player_texture[1][1] = (SDL_Texture *)loadTextures(theGame, "redman/up.png");
    theGame->player_texture[1][2] = (SDL_Texture *)loadTextures(theGame, "redman/right.png");
    theGame->player_texture[1][3] = (SDL_Texture *)loadTextures(theGame, "redman/left.png");
    /*PLAYER ID 2*/
    theGame->player_texture[2][0] = (SDL_Texture *)loadTextures(theGame, "redman/down.png");
    theGame->player_texture[2][1] = (SDL_Texture *)loadTextures(theGame, "redman/up.png");
    theGame->player_texture[2][2] = (SDL_Texture *)loadTextures(theGame, "redman/right.png");
    theGame->player_texture[2][3] = (SDL_Texture *)loadTextures(theGame, "redman/left.png");
    /*PLAYER ID 3*/
    theGame->player_texture[3][0] = (SDL_Texture *)loadTextures(theGame, "pin2.png");
    /*BOWLING BALLS*/
    theGame->bomb_texture[0] = (SDL_Texture *)loadTextures(theGame, "Bowling_Ball_BLue.png");
    theGame->bomb_texture[1] = (SDL_Texture *)loadTextures(theGame, "Bowling_Ball_Purple.png");
    theGame->bomb_texture[2] = (SDL_Texture *)loadTextures(theGame, "Bowling_Ball_Red.png");
    theGame->bomb_texture[3] = (SDL_Texture *)loadTextures(theGame, "Bowling_Ball_Yellow.png");
    theGame->bombExplosion_texture = (SDL_Texture *)loadTextures(theGame, "FIRE.png");
    /*WALLS*/
    theGame->textureWall = (SDL_Texture *)loadTextures(theGame, "wall.png");
    SDL_FreeSurface(theGame->window_surface);
    // Load player sprites
    theGame->pSprites = GetPlayerSprite();
    // Load bomb sprites
    loadBomb();
}
PRIVATE void UpdatePlayerTextures(Game theGame)
{
    // renders player**** EMIL TESTAR HÄR*****
    static int updateSprite[4] = {0};
    static int spriteTimer[4] = {0};
    char moveD = theGame->player[theGame->playerID].moveDirection;
    int dummyPosY = HEIGHT/2 - (theGame->player->height)/2;
    int dummyPosX = WIDTH/2 - (theGame->player->width)/2;

    if (spriteTimer[theGame->playerID] > 10)
        spriteTimer[theGame->playerID] = 0; // Vi får komma på en bra timing för animationsuppdatering alt. en bättre lösning.
    SDL_Rect playerRect0 = {theGame->player[theGame->playerID].xPos, theGame->player[theGame->playerID].yPos, theGame->player->width, theGame->player->height};
    SDL_Rect playerRect2 = {dummyPosX, dummyPosY, theGame->player->width, theGame->player->height};
    SDL_RenderCopyEx(theGame->renderer, theGame->player_texture[2][0], &theGame->pSprites.BowlerManVert[0], &playerRect2, 0, NULL, 0);
    if (moveD == 'w')
    {
        SDL_RenderCopyEx(theGame->renderer, theGame->player_texture[theGame->playerID][1], &theGame->pSprites.BowlerManVert[updateSprite[theGame->playerID]], &playerRect0, 0, NULL, 0);
    }
    else if (moveD == 'a')
    {
        SDL_RenderCopyEx(theGame->renderer, theGame->player_texture[theGame->playerID][3], &theGame->pSprites.BowlerManHori[updateSprite[theGame->playerID]], &playerRect0, 0, NULL, 0);
    }
    else if (moveD == 's')
    {
        SDL_RenderCopyEx(theGame->renderer, theGame->player_texture[theGame->playerID][0], &theGame->pSprites.BowlerManVert[updateSprite[theGame->playerID]], &playerRect0, 0, NULL, 0);
    }
    else if (moveD == 'd')
    {
        SDL_RenderCopyEx(theGame->renderer, theGame->player_texture[theGame->playerID][2], &theGame->pSprites.BowlerManHori[updateSprite[theGame->playerID]], &playerRect0, 0, NULL, 0);
    }
    else
        SDL_RenderCopyEx(theGame->renderer, theGame->player_texture[theGame->playerID][0], &theGame->pSprites.BowlerManVert[0], &playerRect0, 0, NULL, 0);

    if (spriteTimer[theGame->playerID]++ % 5 == 0 && moveD != '0')
        updateSprite[theGame->playerID]++;
    if (updateSprite[theGame->playerID] > 7)
        updateSprite[theGame->playerID] = 0;
    //printf("%c ", moveD);
}

PUBLIC void destroyGame(Game theGame)
{
    SDL_DestroyTexture(theGame->background);
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 3; j++){
            SDL_DestroyTexture(theGame->player_texture[i][j]);
            }
    }
    SDL_DestroyRenderer(theGame->renderer);
    SDL_DestroyWindow(theGame->window);
    SDL_Quit();
}


void initExplosionPosition(Game theGame)
{
    int tilesize = 50;
    theGame->explosionPosition[0].y = theGame->bombs[theGame->playerID].position.y;
    theGame->explosionPosition[0].x = theGame->bombs[theGame->playerID].position.x;
    theGame->explosionPosition[0].h = 50;
    theGame->explosionPosition[0].w = 50;

    theGame->explosionPosition[1].y = theGame->bombs[theGame->playerID].position.y + tilesize;
    theGame->explosionPosition[1].x = theGame->bombs[theGame->playerID].position.x;
    theGame->explosionPosition[1].h = 50;
    theGame->explosionPosition[1].w = 50;

    theGame->explosionPosition[2].y = theGame->bombs[theGame->playerID].position.y - tilesize;
    theGame->explosionPosition[2].x = theGame->bombs[theGame->playerID].position.x;
    theGame->explosionPosition[2].h = 50;
    theGame->explosionPosition[2].w = 50;

    theGame->explosionPosition[3].y = theGame->bombs[theGame->playerID].position.y;
    theGame->explosionPosition[3].x = theGame->bombs[theGame->playerID].position.x + tilesize;
    theGame->explosionPosition[3].h = 50;
    theGame->explosionPosition[3].w = 50;

    theGame->explosionPosition[4].y = theGame->bombs[theGame->playerID].position.y;
    theGame->explosionPosition[4].x = theGame->bombs[theGame->playerID].position.x - tilesize;
    theGame->explosionPosition[4].h = 50;
    theGame->explosionPosition[4].w = 50;
}