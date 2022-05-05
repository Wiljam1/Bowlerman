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

struct data
{ // data sent via UDP
    int x;
    int y;
    int status;
    int playerID;
    char moveDirection;
    int placeBomb;
};

//Blir inte det här globala variabler typ?
UDPsocket sd;
IPaddress srvadd;
UDPpacket *p;
UDPpacket *p2; // behövs egentligen bara en pekare.
struct data udpData = {0, 0, 0, 0};

//Funktionsprototyper ska väl ligga i .h-filen? ---- Tror inte om de är private då de endast används i denna fil? kanske inte spelar någon roll
//Loading all game textures
PRIVATE void loadAllTextures(Game theGame);
// Updating textures depending on movement
PRIVATE void UpdatePlayerTextures(Game theGame);
//Render wall textures
PRIVATE void renderWalls(Game theGame);
void initExplosionPosition(Game theGame, int playerID);
PRIVATE void tryToPlaceBomb(Game theGame, int playerID);

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

    // initiera SDL NET
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

    if (!((p = SDLNet_AllocPacket(sizeof(struct data) + 1)) && (p2 = SDLNet_AllocPacket(sizeof(struct data) + 1))))
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
    // get playerID via UDP

    // 1st: send info to UDP-server
    memcpy(p->data, &udpData, sizeof(struct data) + 1);
    p->len = sizeof(struct data) + 1;
    p->address.host = srvadd.host; /* Set the destination host */
    p->address.port = srvadd.port; /* And destination port */
    SDLNet_UDP_Send(sd, -1, p);

    // 2nd: receive info from UDP-server
    while (!SDLNet_UDP_Recv(sd, p2))
        ; // spin-lock tills received info from UDP-server
    memcpy(&udpData, (char *)p2->data, sizeof(struct data));
    theGame->playerIDLocal = udpData.playerID;
    printf("UDP Packet incoming %d\n", udpData.playerID);
    printf("%d", theGame->playerIDLocal);

    // detta ska ändras via servern sen.
    theGame->playerAmount = PLAYERAMOUNT;

    // allow bomb placement init
    for (int i = 0; i < 4; i++)
    {
        theGame->allowBombPlacement[i] = 1;
    }

    // init x amount of players
    theGame->player[0] = initPlayer(70, 140, 0); // sets x and y coordinates and resets values.

    if (theGame->playerAmount > 1)
    {
        theGame->player[1] = initPlayer(1076, 140, 1); // sets x and y coordinates and resets values.
    }
    if (theGame->playerAmount > 2)
    {
        theGame->player[2] = initPlayer(70, 870, 2); // sets x and y coordinates and resets values.
    }
    if (theGame->playerAmount > 3)
    {
        theGame->player[3] = initPlayer(1076, 870, 3); // sets x and y coordinates and resets values.
    }

    // Init walls / map
    float wallwidth = WIDTH/17; // Vet inte hur vi ska bestämma dehär variablerna riktigt,
    int wallheight = WIDTH/17;  // Om de ens kommer användas, väggarna kommer ju alltid vara den här storleken?
    for (int i = 0; i < WALLAMOUNT; i++)
    {
        theGame->wall[i] = initWalls(WALLAMOUNT, wallwidth, wallheight);
        if (i < 20)
        {
            theGame->wall[i] = wallPlace(i * wallwidth, 100);
        }
        else if (i < 40)
        {
            theGame->wall[i] = wallPlace(wallwidth * (i - 20), HEIGHT - wallheight);
        }
        else if (i < 60)
        {
            theGame->wall[i] = wallPlace(0, (i - 40) * wallheight + 100);
        }
        else if (i < 80)
        {
            theGame->wall[i] = wallPlace(WIDTH - wallwidth, (i - 60) * wallheight + 100);
        }
        else
        {
        }
    }
    //initiering av oförstörbara väggar i mitten av planen.
    int count=-1;
    for (int i=0;i<5;i++)
    {
        for(int j=0;j<7;j++)
        {
            count ++;
            theGame->wall[count+WALLAMOUNT] = initWalls(WALLAMOUNT*3, wallwidth, wallheight);
            theGame->wall[count+WALLAMOUNT] = wallPlace(j*140+140, i*140+240);
        }
    }
    //initiering av förstörbara väggar i planen. 
    count=39;
    for (int i=1;i<12;i++)
    {
        for(int j=0;j<9;j++)
        {
            count ++;
            theGame->wall[count+WALLAMOUNT] = initWalls(WALLAMOUNT*3, wallwidth, wallheight);
            theGame->wall[count+WALLAMOUNT] = wallPlace(j*70+280, i*70+100);
        }
        i+=9;
    }
    for (int i=2;i<12;i++)
    {
        for(int j=0;j<6;j++)
        {
            count ++;
            theGame->wall[count+WALLAMOUNT] = initWalls(WALLAMOUNT*3, wallwidth, wallheight);
            theGame->wall[count+WALLAMOUNT] = wallPlace(j*140+210, i*70+100);
        }
        i+=7;
    }
    for (int i=3;i<12;i++)
    {
        for(int j=0;j<13;j++)
        {
            count ++;
            theGame->wall[count+WALLAMOUNT] = initWalls(WALLAMOUNT*3, wallwidth, wallheight);
            theGame->wall[count+WALLAMOUNT] = wallPlace(j*70+140, i*70+100);
        }
        i+=5;
    }
    for (int i=4;i<9;i++)
    {
        for(int j=0;j<8;j++)
        {
            count ++;
            theGame->wall[count+WALLAMOUNT] = initWalls(WALLAMOUNT*3, wallwidth, wallheight);
            theGame->wall[count+WALLAMOUNT] = wallPlace(j*140+70, i*70+100);
        }
        i++;
    }
    for (int i=5;i<8;i++)
    {
        for(int j=0;j<15;j++)
        {
            count ++;
            theGame->wall[count+WALLAMOUNT] = initWalls(WALLAMOUNT*3, wallwidth, wallheight);
            theGame->wall[count+WALLAMOUNT] = wallPlace(j*70+70, i*70+100);
        }
        i++;
    }
}

void tryToPlaceBomb(Game theGame, int playerID)
{
    if (theGame->allowBombPlacement[playerID] == 1) // man måste veta vilken player här
    {
        theGame->allowBombPlacement[playerID] = 0;
        theGame->bombs[playerID] = initBomb(playerID);
        theGame->bombs[playerID].position.y = correctBowlingBallPos(getPlayerYPosition(theGame->player[playerID]) + 56) - 40;
        theGame->bombs[playerID].position.x = correctBowlingBallPos(getPlayerXPosition(theGame->player[playerID]) + 8);
        theGame->bombs[playerID].timervalue = initbowlingballtimer(SDL_GetTicks(), 3000, playerID); // också viktigt att veta vilken player
        theGame->bombs[playerID].timerinit = 1;           //detta värdet borde skickas som data till andra players
        theGame->bombs[playerID].placedBombRestriction = 1;
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
                tryToPlaceBomb(theGame, theGame->playerIDLocal);
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

PRIVATE void manageUDP(Game theGame)
{
    static int flag;
    udpData.moveDirection = theGame->player[theGame->playerIDLocal].moveDirection;
    int x_posOld = theGame->player[theGame->playerIDLocal].xPosOld;
    int y_posOld = theGame->player[theGame->playerIDLocal].yPosOld;
    int x_pos = theGame->player[theGame->playerIDLocal].xPos;
    int y_pos = theGame->player[theGame->playerIDLocal].yPos;

    static int bombUDPtimer=0;
    udpData.placeBomb=0;
    if(theGame->bombs[theGame->playerIDLocal].timerinit==1){  
        if(bombUDPtimer<1)    //vi ser till att skicka 1 packets, för om vi skickar för mycket blir det buggigt pga delay med UDP i slutet
        {
            udpData.placeBomb=1;
            bombUDPtimer++;
        }
        else {
            udpData.placeBomb=0;
        }
    }
    else{
        bombUDPtimer=0;
        udpData.placeBomb=0;
    }

    // send data if movement or bomb-placement
    if (x_posOld != x_pos || y_posOld != y_pos || flag == 1 || udpData.placeBomb==1)
    {
        printf("%d %d\n", (int)x_pos, (int)y_pos);
        udpData.playerID = theGame->playerIDLocal;
        
        
        udpData.x = x_pos;
        udpData.y = y_pos;
        memcpy(p->data, &udpData, sizeof(struct data) + 1);
        // fwrite(&udpData, sizeof(struct data), 1, p->data);
        p->len = sizeof(struct data) + 1;
        // sprintf((char *)p->data, "%d %d\n", (int) x_pos, (int) y_pos);
        p->address.host = srvadd.host; /* Set the destination host */
        p->address.port = srvadd.port; /* And destination port */
        // p->len = strlen((char *)p->data) + 1;
        SDLNet_UDP_Send(sd, -1, p);
        theGame->player[theGame->playerIDLocal].xPosOld = x_pos; // Fixade oldxpos
        theGame->player[theGame->playerIDLocal].yPosOld = y_pos;
        flag = 0;
    }
    if (udpData.moveDirection != '0'){
        flag = 1;
    }
    // receive data
    if (SDLNet_UDP_Recv(sd, p2))
    {
        int a, b;
        // sscanf((char * )p2->data, "%d %d\n", &a, &b);
        memcpy(&udpData, (char *)p2->data, sizeof(struct data));
        int playerID = udpData.playerID;
        theGame->player[playerID].xPos = udpData.x;
        theGame->player[playerID].yPos = udpData.y;
        if (udpData.placeBomb==1){ 
            tryToPlaceBomb(theGame, playerID);
        }

        theGame->player[playerID].moveDirection = udpData.moveDirection;
        theGame->player[playerID].id = udpData.playerID;
        printf("UDP Packet incoming, x,y-coord: %d %d of player %d\n", udpData.x, udpData.y, udpData.playerID);
    }
}

// game loop
PUBLIC void gameUpdate(Game theGame)
{
    // Initialize
    Player player[MAXPLAYERS]; // declares x-amounts of players
    initGame(theGame);         // initializes startvalues. coordinates etc.

    // Game Loop:
    bool done = false;
    while (!done)
    {
        // Check for events
        done = checkEvents(theGame);
        // Process events (time based stuff)
        for (int i = 0; i < 4; i++){
            if (theGame->bombs[i].timerinit == 1){
                theGame->bombs[i].timervalue = initbowlingballtimer(0, 3000, i);
                if (theGame->bombs[i].timervalue == 1){
                    theGame->bombs[i].timerinit = 0;
                    theGame->bombs[i].explosioninit = 0;
                    initExplosionPosition(theGame, i);
                    initbowlingballtimer(SDL_GetTicks(), 1000, i);
                }
            }
        }
        for (int i = 0; i < 4; i++){
            if (theGame->bombs[i].explosioninit == 0){
                theGame->bombs[i].explosioninit = initbowlingballtimer(0, 1000, i);
                for(int j=139;j<250;j++){
                    if(theGame->wall[j].destroyedWall == 0){
                        theGame->wall[j].destroyedWall = testCollisionWithDestroyableWalls(theGame, j, i);
                    }
                }
                if (theGame->bombs[i].explosioninit == 1){
                    theGame->allowBombPlacement[i] = 1;
                }
            }
        }
        // process();

        // Collisiondetection
        collisionDetect(theGame);
        testCollosionWithBombs(theGame);
        testCollosionWithExplosion(theGame);
        testCollisionWithWalls(theGame);

        // Send/receive data to server
        manageUDP(theGame);

        // render display
        renderTextures(theGame);

        SDL_Delay(1000 / 60); // man behöver ta minus här för att räkna in hur lång tid spelet tar att exekvera
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
    int id = theGame->playerIDLocal;

    // clear renderer
    SDL_RenderClear(renderer);

    // updates/renders background
    SDL_Rect backRect = {0, 100, WIDTH, HEIGHT};
    SDL_RenderCopy(renderer, theGame->background, NULL, &backRect);
    renderWalls(theGame);
    // bubble-sort the players y-position into the array "renderOrder"
    // arraySorter(player, theGame->playerAmount, renderOrder);

    // render bombs and explosion
    for (int i = 0; i < 4; i++)
    {
        if (theGame->bombs[i].timervalue == 0)
        {
            SDL_RenderCopy(renderer, theGame->bomb_texture[i], &bowlingballAnimation[0], &theGame->bombs[i].position);
        }
        if (theGame->bombs[i].explosioninit == 0)
        {
            for (int j = 0; j < 5; j++)
            {
                if(testCollisionExplosionWithWalls(theGame, j) == 0)
                {
                    SDL_RenderCopy(theGame->renderer, theGame->bombExplosion_texture, &bowlingballAnimation[0], &theGame->explosionPosition[i][j]);
                }
            }
        }
    }

    UpdatePlayerTextures(theGame);
    
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
    theGame->player_texture[1][0] = (SDL_Texture *)loadTextures(theGame, "purpleman/down.png");
    theGame->player_texture[1][1] = (SDL_Texture *)loadTextures(theGame, "purpleman/up.png");
    theGame->player_texture[1][2] = (SDL_Texture *)loadTextures(theGame, "purpleman/right.png");
    theGame->player_texture[1][3] = (SDL_Texture *)loadTextures(theGame, "purpleman/left.png");
    /*PLAYER ID 2*/
    theGame->player_texture[2][0] = (SDL_Texture *)loadTextures(theGame, "redman/down.png");
    theGame->player_texture[2][1] = (SDL_Texture *)loadTextures(theGame, "redman/up.png");
    theGame->player_texture[2][2] = (SDL_Texture *)loadTextures(theGame, "redman/right.png");
    theGame->player_texture[2][3] = (SDL_Texture *)loadTextures(theGame, "redman/left.png");
    /*PLAYER ID 3*/
    theGame->player_texture[3][0] = (SDL_Texture *)loadTextures(theGame, "yellowman/down.png");
    theGame->player_texture[3][1] = (SDL_Texture *)loadTextures(theGame, "yellowman/up.png");
    theGame->player_texture[3][2] = (SDL_Texture *)loadTextures(theGame, "yellowman/right.png");
    theGame->player_texture[3][3] = (SDL_Texture *)loadTextures(theGame, "yellowman/left.png");
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
    theGame->textureWall[3] = (SDL_Texture *)loadTextures(theGame, "wall.png");
    SDL_FreeSurface(theGame->window_surface);
    // Load player sprites
    theGame->pSprites = GetPlayerSprite();
    // Load bomb sprites
    loadBomb();
}

PRIVATE void UpdatePlayerTextures(Game theGame)
{
    /*Keeps track of what sprites to load and at what timing*/
    static Uint8 updateSprite[4] = {0};
    static Uint8 spriteTimer[4] = {0};
    Uint8 spriteChoice[4] = {0}; 
    
    /*Init all player rectangles*/
    SDL_Rect playerRect[4];
    for(int i=0; i<theGame->playerAmount; i++)
    {
        playerRect[i].x = theGame->player[i].xPos;
        playerRect[i].y = theGame->player[i].yPos;
        playerRect[i].w= theGame->player->width;
        playerRect[i].h = theGame->player->height;
    }
    /*Managing sprite updates*/
    for(int i=0; i < theGame->playerAmount; i++)
    {
        if (theGame->player[i].isDead == false){
            if (spriteTimer[i] > 10){ //Slowing down sprite updates
                spriteTimer[i] = 0; 
            }
                switch (theGame->player[i].moveDirection)
                {
                    case 'w': 
                        spriteChoice[i] = 1;
                        SDL_RenderCopy(theGame->renderer, theGame->player_texture[i][spriteChoice[i]], &theGame->pSprites.BowlerManVert[updateSprite[i]], &playerRect[i]);
                        break;
                    case 'a': spriteChoice[i] = 3;
                        SDL_RenderCopy(theGame->renderer, theGame->player_texture[i][spriteChoice[i]], &theGame->pSprites.BowlerManHori[updateSprite[i]], &playerRect[i]);
                        break;
                    case 's': spriteChoice[i] = 0;
                        SDL_RenderCopy(theGame->renderer, theGame->player_texture[i][spriteChoice[i]], &theGame->pSprites.BowlerManVert[updateSprite[i]], &playerRect[i]);
                        break;
                    case 'd': spriteChoice[i] = 2;
                        SDL_RenderCopy(theGame->renderer, theGame->player_texture[i][spriteChoice[i]], &theGame->pSprites.BowlerManHori[updateSprite[i]], &playerRect[i]);
                        break;
                    case '0':
                    default: spriteChoice[i] = 0;
                        SDL_RenderCopy(theGame->renderer, theGame->player_texture[i][spriteChoice[i]], &theGame->pSprites.BowlerManVert[0], &playerRect[i]);
                        break;
                }
                if (spriteTimer[i]++ % 5 == 0 && theGame->player[i].moveDirection != '0'){ // Slowing down sprite updates
                        updateSprite[i]++;
                }
                if (updateSprite[i] > 7){
                        updateSprite[i] = 0;
                }
        }
        else{
            getStartPos(&theGame->player[i]); // If player is dead it respawns at starting pos
            theGame->player[i].isDead = false;
        }
    }
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

PRIVATE void renderWalls(Game theGame)
{
    // Draw walls
    for (int i = 77; i >= 0; i--)
    {
        SDL_Rect wallRect = {theGame->wall[i].x, theGame->wall[i].y, theGame->wall[i].w, theGame->wall[i].h};
        /* LONG WALLS*/
        if (i < 36)
        {
            SDL_RenderCopy(theGame->renderer, theGame->textureWall[0], NULL, &wallRect);
        }
        else if (i < 76)
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
    for (int i=0;i<36;i++)
    {   
        SDL_Rect wallRect = {theGame->wall[i+WALLAMOUNT].x, theGame->wall[i+WALLAMOUNT].y, theGame->wall[i+WALLAMOUNT].w, theGame->wall[i+WALLAMOUNT].h};
        SDL_RenderCopy(theGame->renderer, theGame->textureWall[2], NULL, &wallRect);        
    }
    for (int i=139;i<250;i++)
    {   
        if (theGame->wall[i].destroyedWall == 0)
        {
            SDL_Rect wallRect = {theGame->wall[i].x, theGame->wall[i].y, theGame->wall[i].w, theGame->wall[i].h};
            SDL_RenderCopy(theGame->renderer, theGame->textureWall[3], NULL, &wallRect);   
        }     
    }


}

//Kanske borde vara i bomb.c?
void initExplosionPosition(Game theGame, int playerID)
{
    int tilesize = 66, diff=2; //Borde sparas i en struct för att komma åt värdet vid collisiondetection?

    for(int i = 0; i < 5; i++){
        theGame->explosionPosition[playerID][i].h = tilesize;
        theGame->explosionPosition[playerID][i].w = tilesize;
    }

    // I framtiden ska man väl kunna ha större explosionsradius än det vanliga?
    // Man kanske får initiera flera positioner från början men endast rendera/ha collision med de som ska visas
    int j = 0;
    theGame->explosionPosition[playerID][j].y = theGame->bombs[playerID].position.y + diff;
    theGame->explosionPosition[playerID][j].x = theGame->bombs[playerID].position.x + diff;
    j++;
    if (testPossibilityToExplode(theGame, playerID, j) == 1)
    {
        theGame->explosionPosition[playerID][j].y = theGame->bombs[playerID].position.y + tilesize + diff*3; // Neråt
        theGame->explosionPosition[playerID][j].x = theGame->bombs[playerID].position.x + diff;
    }
    j++;
    if (testPossibilityToExplode(theGame, playerID, j) == 1)
    {
        theGame->explosionPosition[playerID][j].y = theGame->bombs[playerID].position.y - tilesize - diff;    // UPP
        theGame->explosionPosition[playerID][j].x = theGame->bombs[playerID].position.x + diff;
    }
    j++;
    if (testPossibilityToExplode(theGame, playerID, j) == 1)
    {
        theGame->explosionPosition[playerID][j].y = theGame->bombs[playerID].position.y + diff;             //Höger
        theGame->explosionPosition[playerID][j].x = theGame->bombs[playerID].position.x + tilesize + diff*3;
    }
    j++;
    if (testPossibilityToExplode(theGame, playerID, j) == 1)
    {
        theGame->explosionPosition[playerID][j].y = theGame->bombs[playerID].position.y + diff;             //Vänster
        theGame->explosionPosition[playerID][j].x = theGame->bombs[playerID].position.x - tilesize - diff;
    }
}