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
#include "sorter.h"
#include "bomb.h"

#define PUBLIC /* empty */
#define PRIVATE static
#define LENGTH 100


void initExplosionPosition(Game theGame, int playerID);


struct data {           //data sent via UDP
   int x;
   int y;
   int status;
   int playerID;
};

UDPsocket sd;
IPaddress srvadd;
UDPpacket *p;
UDPpacket *p2;  //behövs egentligen bara en pekare.
struct data udpData = {0, 0, 0, 0};

PRIVATE void loadAllTextures(Game theGame);
PRIVATE void UpdatePlayerTextures(Game theGame);
PRIVATE void renderWalls(Game theGame);
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
    
    //initiera SDL NET
    if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    if (!(sd = SDLNet_UDP_Open(0)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    /* Resolve server name  */
	if (SDLNet_ResolveHost(&srvadd, "127.0.0.1", 2000) == -1)
	{
		fprintf(stderr, "SDLNet_ResolveHost(192.0.0.1 2000): %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    if (!((p = SDLNet_AllocPacket(sizeof(struct data)+1))&& (p2 = SDLNet_AllocPacket(sizeof(struct data)+1))))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
    return theGame;
}

// initializes startvalues for game
void initGame(Game theGame)
{
    // Loading textures from file
    loadAllTextures(theGame);
    // check server what ID you have.
    // getPlayerID();
    //get playerID via UDP
    //1st: send info to UDP-server
    memcpy(p->data, &udpData, sizeof(struct data)+1);
    p->len = sizeof(struct data)+1;   
    p->address.host = srvadd.host;	/* Set the destination host */
    p->address.port = srvadd.port;	/* And destination port */
    SDLNet_UDP_Send(sd, -1, p);
<<<<<<< Updated upstream

    //2nd: receive playerID from UDP-server
=======
    //2nd: receive info from UDP-server
>>>>>>> Stashed changes
    while(!SDLNet_UDP_Recv(sd, p2));    //spin-lock tills received info from UDP-server
    memcpy(&udpData, (char * ) p2->data, sizeof(struct data));
    theGame->playerID= udpData.playerID;
    printf("UDP Packet incoming %d\n", udpData.playerID);
    //theGame->playerID = 0;
    printf("CRASH");
    printf("%d", theGame->playerID);
   
    // detta ska ändras via servern sen.
    theGame->playerAmmount = 4;

    //allow bomb placement init
    for (int i=0;i<4;i++)
    {
        theGame->allowBombPlacement[i] = 1;
    }
    // inits x-amount of players
    theGame->player[0] = initPlayer(200, 200); // sets x and y coordinates and resets values.
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
    float wallwidth = 70;  // Vet inte hur vi ska bestämma dehär variablerna riktigt,
    int wallheight = 70; // Om de ens kommer användas
    for (int i = 0; i < WALLAMOUNT; i++)
    {
        theGame->wall[i] = initWalls(WALLAMOUNT, wallwidth, wallheight);
        if (i < 20)
        {
            theGame->wall[i] = wallPlace(i * wallwidth, 100);
        }
        else if (i < 40)
        {
            theGame->wall[i] = wallPlace(wallwidth *(i - 20), HEIGHT - wallheight);
        }
        else if (i < 60)
        {
            theGame->wall[i] = wallPlace(0, (i - 40) * wallheight + 100);
        }
        else if (i < 80)
        {
            theGame->wall[i] = wallPlace(WIDTH - wallwidth, (i - 60) * wallheight +100);
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
                if(theGame->allowBombPlacement[theGame->playerID] == 1) // man måste veta vilken player här
                {
                    theGame->allowBombPlacement[theGame->playerID] = 0;
                    theGame->bombs[theGame->playerID] = initBomb(theGame->playerID);
                    theGame->bombs[theGame->playerID].position.y = getPlayerYPosition(theGame->player[theGame->playerID]) + 56;
                    theGame->bombs[theGame->playerID].position.x = getPlayerXPosition(theGame->player[theGame->playerID]) + 8;
                    theGame->bombs[theGame->playerID].timervalue = initbowlingballtimer(SDL_GetTicks(), 3000, theGame->playerID); //också viktigt att veta vilken player
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

PRIVATE void manageUDP(Game theGame)
{
    int x_posOld = theGame->player[theGame->playerID].xPosOld;
    int y_posOld = theGame->player[theGame->playerID].yPosOld;
    int x_pos = theGame->player[theGame->playerID].xPos;
    int y_pos = theGame->player[theGame->playerID].yPos;

    // send positions  
    if(x_posOld != x_pos || y_posOld != y_pos){
        printf("%d %d\n", (int) x_pos, (int) y_pos);
        udpData.playerID=theGame->playerID;
        udpData.x = x_pos;
        udpData.y = y_pos;
        memcpy(p->data, &udpData, sizeof(struct data)+1);
        //fwrite(&udpData, sizeof(struct data), 1, p->data);
        p->len = sizeof(struct data)+1;
        //sprintf((char *)p->data, "%d %d\n", (int) x_pos, (int) y_pos);    
        p->address.host = srvadd.host;	/* Set the destination host */
        p->address.port = srvadd.port;	/* And destination port */
        //p->len = strlen((char *)p->data) + 1;
        SDLNet_UDP_Send(sd, -1, p);
        x_posOld = x_pos;
        y_posOld = y_pos;
    }      
        
    //receive data
    if (SDLNet_UDP_Recv(sd, p2)){
        int a, b; 
        //sscanf((char * )p2->data, "%d %d\n", &a, &b);
        memcpy(&udpData, (char * ) p2->data, sizeof(struct data));
        int playerID = udpData.playerID;
        theGame->player[playerID].xPos= udpData.x;
        theGame->player[playerID].yPos= udpData.y;
        printf("UDP Packet incoming, x,y-coord: %d %d of player %d\n", udpData.x, udpData.y, udpData.playerID);
    }
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
        for(int i=0;i < 4;i++){
            if (theGame->bombs[i].timerinit == 1){
                theGame->bombs[i].timervalue = initbowlingballtimer(0, 3000, i);
                if(theGame->bombs[i].timervalue == 1){
                    theGame->bombs[i].timerinit = 0;
                    theGame->bombs[i].explosioninit = 0;
                    initExplosionPosition(theGame, i);
                    initbowlingballtimer(SDL_GetTicks(), 1000, i);
                }
            }
        }
        for(int i=0;i < 4;i++){
            if(theGame->bombs[i].explosioninit == 0){
                theGame->bombs[i].explosioninit = initbowlingballtimer(0, 1000, i);
                if(theGame->bombs[i].explosioninit == 1){
                    theGame->allowBombPlacement[i] = 1;
                }
            }
        }
        // process();
        
        // Collisiondetection
        collisionDetect(theGame);

        // Send/receive data to server
        manageUDP(theGame);

        // render display
        renderTextures(theGame);

        SDL_Delay(1000/60); // man behöver ta minus här för att räkna in hur lång tid spelet tar att exekvera
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
    SDL_Rect backRect = {0, 100, WIDTH, HEIGHT};
    SDL_RenderCopy(renderer, theGame->background, NULL, &backRect);
    renderWalls(theGame);
    //bubble-sort the players y-position into the array "renderOrder"
    //arraySorter(player, theGame->playerAmmount, renderOrder);

    // Updating textures depending on movement
    UpdatePlayerTextures(theGame);
     // render bombs and explosion
    for(int i=0;i < 4;i++){
        if (theGame->bombs[i].timervalue == 0){
            SDL_RenderCopy(renderer, theGame->bomb_texture[i], &bowlingballAnimation[0], &theGame->bombs[i].position);
        }
        if (theGame->bombs[i].explosioninit == 0){
            for(int j=0;j<5;j++){
                SDL_RenderCopy(theGame->renderer, theGame->bombExplosion_texture, &bowlingballAnimation[ 0 ], &theGame->explosionPosition[i][j]);
            }
        }
    }
    // Draw GUI last (top of screenlayers)

    SDL_RenderPresent(renderer); // present renderer
}
PRIVATE void loadAllTextures(Game theGame)
{
    // loads in textures
    theGame->background = (SDL_Texture *)loadTextures(theGame, "alley.png");
    /*PLAYER ID 0*/
    theGame->player_texture[0][0] = (SDL_Texture *)loadTextures(theGame, "blueman/down.png");
    theGame->player_texture[0][1] = (SDL_Texture *)loadTextures(theGame, "blueman/up.png");
    theGame->player_texture[0][2] = (SDL_Texture *)loadTextures(theGame, "blueman/right.png");
    theGame->player_texture[0][3] = (SDL_Texture *)loadTextures(theGame, "blueman/left.png");
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
    theGame->player_texture[3][0] = (SDL_Texture *)loadTextures(theGame, "redman/down.png");
    theGame->player_texture[3][1] = (SDL_Texture *)loadTextures(theGame, "redman/up.png");
    theGame->player_texture[3][2] = (SDL_Texture *)loadTextures(theGame, "redman/right.png");
    theGame->player_texture[3][3] = (SDL_Texture *)loadTextures(theGame, "redman/left.png");
    /*BOWLING BALLS*/
    theGame->bomb_texture[0] = (SDL_Texture *)loadTextures(theGame, "Bowling_Ball_BLue.png");
    theGame->bomb_texture[1] = (SDL_Texture *)loadTextures(theGame, "Bowling_Ball_Purple.png");
    theGame->bomb_texture[2] = (SDL_Texture *)loadTextures(theGame, "Bowling_Ball_Red.png");
    theGame->bomb_texture[3] = (SDL_Texture *)loadTextures(theGame, "Bowling_Ball_Yellow.png");
    theGame->bombExplosion_texture = (SDL_Texture *)loadTextures(theGame, "FIRE.png");
    /*WALLS*/
    theGame->textureWall[0] = (SDL_Texture *)loadTextures(theGame, "walls/80/straight.png");
    theGame->textureWall[1] = (SDL_Texture *)loadTextures(theGame, "walls/80/corner.png");
    theGame->textureWall[2] = (SDL_Texture *)loadTextures(theGame, "walls/80/center.png");
    SDL_FreeSurface(theGame->window_surface);
    // Load player sprites
    theGame->pSprites = GetPlayerSprite();
    // Load bomb sprites
    loadBomb();
}
PRIVATE void UpdatePlayerTextures(Game theGame)
{
    // renders player**** EMIL TESTAR HÄR*****
    static Uint8 updateSprite[4] = {0};
    static Uint8 spriteTimer[4] = {0};
    char moveD = theGame->player[theGame->playerID].moveDirection;
    int dummyPosY = 500;
    int dummyPosX = WIDTH/2 - 100/2;
    int spriteChoice;
    
    if (spriteTimer[theGame->playerID] > 10)
        spriteTimer[theGame->playerID] = 0; // Vi får komma på en bra timing för animationsuppdatering alt. en bättre lösning.
    SDL_Rect playerRect[4] = {{theGame->player[0].xPos, theGame->player[0].yPos, theGame->player->width, theGame->player->height},
                             {theGame->player[1].xPos, theGame->player[1].yPos, theGame->player->width, theGame->player->height},
                             {theGame->player[2].xPos, theGame->player[2].yPos, theGame->player->width, theGame->player->height},
                             {theGame->player[3].xPos, theGame->player[3].yPos, theGame->player->width, theGame->player->height}};

    SDL_Rect playerRect2 = {dummyPosX, dummyPosY, theGame->player->width, theGame->player->height};
    SDL_RenderCopyEx(theGame->renderer, theGame->player_texture[2][0], &theGame->pSprites.BowlerManVert[0], &playerRect2, 0, NULL, 0);
    if (moveD == 'w')
        SDL_RenderCopyEx(theGame->renderer, theGame->player_texture[theGame->playerID][1], &theGame->pSprites.BowlerManVert[updateSprite[theGame->playerID]], &playerRect[theGame->playerID], 0, NULL, 0);
    else if (moveD == 'a')
        SDL_RenderCopyEx(theGame->renderer, theGame->player_texture[theGame->playerID][3], &theGame->pSprites.BowlerManHori[updateSprite[theGame->playerID]], &playerRect[theGame->playerID], 0, NULL, 0);
    else if (moveD == 's')
        SDL_RenderCopyEx(theGame->renderer, theGame->player_texture[theGame->playerID][0], &theGame->pSprites.BowlerManVert[updateSprite[theGame->playerID]], &playerRect[theGame->playerID], 0, NULL, 0);
    else if (moveD == 'd')
        SDL_RenderCopyEx(theGame->renderer, theGame->player_texture[theGame->playerID][2], &theGame->pSprites.BowlerManHori[updateSprite[theGame->playerID]], &playerRect[theGame->playerID], 0, NULL, 0);
    else 
        SDL_RenderCopyEx(theGame->renderer, theGame->player_texture[theGame->playerID][0], &theGame->pSprites.BowlerManVert[0], &playerRect[theGame->playerID], 0, NULL, 0);

    if (spriteTimer[theGame->playerID]++ % 5 == 0 && moveD != '0')
        updateSprite[theGame->playerID]++;
    if (updateSprite[theGame->playerID] > 7)
        updateSprite[theGame->playerID] = 0;
}

PUBLIC void destroyGame(Game theGame)
{
    SDL_DestroyTexture(theGame->background);
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 3; j++){
            SDL_DestroyTexture(theGame->player_texture[i][j]);
            }
    }
    for (int i = 0; i < 4; i++){   
        SDL_DestroyTexture(theGame->bomb_texture[i]);
    }
    SDL_DestroyTexture(theGame->bombExplosion_texture);
    SDL_DestroyRenderer(theGame->renderer);
    SDL_DestroyWindow(theGame->window);
    SDL_Quit();
}

 PRIVATE void renderWalls(Game theGame)
 {
     //Draw walls
    for (int i = 100; i >= 0; i--)
    {
        SDL_Rect wallRect = {theGame->wall[i].x, theGame->wall[i].y, theGame->wall[i].w, theGame->wall[i].h};
        /* LONG WALLS*/
        if (i < 36)
        {
            SDL_RenderCopy(theGame->renderer, theGame->textureWall[0], NULL, &wallRect);
        }
        else if(i < 76)
        {
            SDL_RenderCopyEx(theGame->renderer, theGame->textureWall[0], NULL, &wallRect, 90, 0, 0);
        }
        /* CORNER WALLS */
        if (i == 0)
        {
            SDL_RenderCopyEx(theGame->renderer, theGame->textureWall[1], NULL, &wallRect, 90, 0, 0);
        }
        if (i == 16)
        {
            SDL_RenderCopyEx(theGame->renderer, theGame->textureWall[1], NULL, &wallRect, 180, 0, 0);
        }
        if (i == 20)
        {
            SDL_RenderCopyEx(theGame->renderer, theGame->textureWall[1], NULL, &wallRect, 0, 0, 0);
        }
        if (i == 36)
        {
            SDL_RenderCopyEx(theGame->renderer, theGame->textureWall[1], NULL, &wallRect, 270, 0, 0);
        }
    }
 }

void initExplosionPosition(Game theGame, int playerID)
{
    int tilesize = 50;
    theGame->explosionPosition[playerID][0].y = theGame->bombs[playerID].position.y;
    theGame->explosionPosition[playerID][0].x = theGame->bombs[playerID].position.x;
    theGame->explosionPosition[playerID][0].h = tilesize;
    theGame->explosionPosition[playerID][0].w = tilesize;

    theGame->explosionPosition[playerID][1].y = theGame->bombs[playerID].position.y + tilesize;
    theGame->explosionPosition[playerID][1].x = theGame->bombs[playerID].position.x;
    theGame->explosionPosition[playerID][1].h = tilesize;
    theGame->explosionPosition[playerID][1].w = tilesize;

    theGame->explosionPosition[playerID][2].y = theGame->bombs[playerID].position.y - tilesize;
    theGame->explosionPosition[playerID][2].x = theGame->bombs[playerID].position.x;
    theGame->explosionPosition[playerID][2].h = tilesize;
    theGame->explosionPosition[playerID][2].w = tilesize;

    theGame->explosionPosition[playerID][3].y = theGame->bombs[playerID].position.y;
    theGame->explosionPosition[playerID][3].x = theGame->bombs[playerID].position.x + tilesize;
    theGame->explosionPosition[playerID][3].h = tilesize;
    theGame->explosionPosition[playerID][3].w = tilesize;

    theGame->explosionPosition[playerID][4].y = theGame->bombs[playerID].position.y;
    theGame->explosionPosition[playerID][4].x = theGame->bombs[playerID].position.x - tilesize;
    theGame->explosionPosition[playerID][4].h = tilesize;
    theGame->explosionPosition[playerID][4].w = tilesize;
}