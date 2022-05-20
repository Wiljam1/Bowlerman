#include "udpClient.h"
#include "bomb.h"
#include "powerup.h"
#define PUBLIC /* empty */
#define PRIVATE static
#define UPDATESPEED 5
#define MAXAMMOUNTOFBOMBS 5   //how many bombs are allowed to be placed by one player
void UDPSetPlayerID(UDPData *u, int id);
void UDPSetXPos(UDPData *u, int x);
void UDPSetYPos(UDPData *u, int y);
void UDPSetScore(UDPData *u, int id, int score);
void UDPSetNoOfLives(UDPData *u, int lives);
int UDPGetPlayerID(UDPData *u);
int UDPGetScore(UDPData *u, int i);
void UDPSetMoveDirection(UDPData *u, char c);
PRIVATE void sendBomb(Game theGame, UDPData *udpData, UDPStruct *udpValues)
{
    int playerID = theGame->playerIDLocal;
    udpData->placeBomb=0;
    static int doPlaceBomb=0;
    static int flagSendBomb[5]={0};

    for(int i=0; i<MAXAMMOUNTOFBOMBS; i++)   //for-loop för vi vill kunna släppa upp till 5 bomber
    {
        if(theGame->bombs[playerID+i*4].timerinit==1){  
            if(flagSendBomb[i]==0)    //vi ser till att skicka 1 packets, för om vi skickar för mycket blir det buggigt pga delay med UDP i slutet
            {
                //udpData->placeBomb=1;
                flagSendBomb[i]=1;
                doPlaceBomb=1;
                //break;
            }
            else {
                //udpData->placeBomb=0;
            }
        }
        else{
            flagSendBomb[i]=0;
            //udpData->placeBomb=0;
        }
    }
    //reset placeBomb (udpData->placeBomb)
    for (int i=0; i<MAXAMMOUNTOFBOMBS; i++)
    {
        if (flagSendBomb[i]==0)
        {
            udpData->placeBomb=0;
        }
    }
    //check if we should placeBomb (udpData->placeBomb)
    for (int i=0; i<MAXAMMOUNTOFBOMBS; i++)
    {
        if(flagSendBomb[i]==1 && doPlaceBomb==1)
        {
            doPlaceBomb=0;
            udpData->placeBomb=1;

        }
    }
}


//Send Data
PRIVATE void sendUDP(Game theGame,UDPData *udpData, UDPStruct *udpValues, int *flag, int *flagSendOnStartup)
{
    int playerID = getLocalID(theGame);
    int x_posOld = playerGetOldXpos(theGame->player[playerID]);
    int y_posOld = playerGetOldYPos(theGame->player[playerID]);
    float x_pos = playerGetXPosition(theGame->player[playerID]);
    float y_pos = playerGetYPosition(theGame->player[playerID]);
    static int oldScore = 0, scoreGUIFlag = 0;

    if(theGame->player[playerID].score != oldScore){
        oldScore = playerGetScore(theGame->player[playerID]);
        scoreGUIFlag = 1;
    }
    // send data if movement or bomb-placement
    UDPSetMoveDirection(udpData, playerGetMoveDirection(theGame->player[playerID]));
    if (abs(x_posOld - x_pos) >= UPDATESPEED || abs(y_posOld - y_pos) >= UPDATESPEED || *flag == 1 || udpData->placeBomb==1 || scoreGUIFlag == 1 || *flagSendOnStartup==1)
    {
        UDPSetPlayerID(udpData, playerID);
        UDPSetXPos(udpData, x_pos);
        UDPSetYPos(udpData, y_pos);
        udpData->powerupsX = 0; // VAD GÖR DENNA?
        UDPSetNoOfLives(udpData, playerGetNoOfLives(theGame->player[playerID]));
        UDPSetScore(udpData, playerID, playerGetScore(theGame->player[playerID]));
        for(int i=0;i<POWERUPAMOUNT;i++)
        {
            if(theGame->powerups[i].sentViaUDP == false)
            {
                udpData->powerupsX = theGame->powerups[i].x;
                udpData->powerupsY = theGame->powerups[i].y;
                udpData->powerupsID = theGame->powerups[i].id;
                udpData->powerupsType = theGame->powerups[i].type;
                theGame->powerups[i].sentViaUDP = true;
                break;
            }     
        }
        memcpy(udpValues->p->data, &(*udpData), sizeof(UDPData) + 1);
        udpValues->p->len = sizeof(UDPData) + 1;
        udpValues->p->address.host = udpValues->srvadd.host; /* Set the destination host */
        udpValues->p->address.port = udpValues->srvadd.port; /* And destination port */
        SDLNet_UDP_Send(udpValues->sd, -1, udpValues->p);
        playerSetOldXPos(&theGame->player[playerID], x_pos);
        playerSetOldYPos(&theGame->player[playerID], y_pos);
        *flag=0;
        scoreGUIFlag = 0;
        *flagSendOnStartup=0;
    }
}
//Recieve Data
PRIVATE void receiveUDP(Game theGame,UDPData *udpData, UDPStruct *udpValues)
{
    if (SDLNet_UDP_Recv(udpValues->sd, udpValues->p2))
    {
        int a, b;
        static int oldScore[4] = {0};
        memcpy(&(*udpData), (char *)udpValues->p2->data, sizeof(struct data));
        int playerID = UDPGetPlayerID(udpData);
        playerSetXPos(&(theGame->player[playerID]), udpData->x);
        playerSetYPos(&(theGame->player[playerID]), udpData->y);
        if (udpData->placeBomb==1){ 
            tryToPlaceBomb(theGame, playerID);
        }
        
        if(udpData->powerupsX != 0)
        {
            theGame->powerups[udpData->powerupsID] = powerupPlace(udpData->powerupsX-WIDTH/119, udpData->powerupsY-WIDTH/119, udpData->powerupsType);
            theGame->powerups[udpData->powerupsID].indestructable = timerForPowerups(SDL_GetTicks(), 1500, udpData->powerupsID);
        }
        playerSetMoveDirection(&(theGame->player[playerID]), udpData->moveDirection);
        
        //ska denna vara här?
        //playerSetID(&(theGame->player[playerID]), udpData->playerID);
        playerSetNoOfLives(&(theGame->player[playerID]), udpData->noOfLives);
        playerSetScore(&(theGame->player[playerID]), udpData->score[playerID]);
        for(int i = 0; i < 4; i++){
            if(udpData->score[i] != oldScore[i]){
                updateScoreFlag(theGame, true);
                oldScore[i] = UDPGetScore(udpData, i);
            }    
        }
    }
}

PUBLIC void manageUDP(Game theGame, UDPData *udpData, UDPStruct *udpValues)
{
    static int flag=0;
    static int flag2=0;
    
    int playerID = getLocalID(theGame);
    //udpData->moveDirection = theGame->player[playerID].moveDirection;
    playerSetMoveDirection(&theGame->player[playerID], playerGetMoveDirection(theGame->player[playerID]));

    //check to see if we should send bomb with UDP
    sendBomb(theGame, udpData, udpValues);
    //flagga för att reset:a movement direction när spelaren står stilla.
    if( udpData->moveDirection != '0'){
       flag2=1;
    }
    if( (udpData->moveDirection == '0') && flag2){
       flag=1;
       flag2=0;
    }

    static int flagSendOnStartup=1;

    //send udp data (if e.g. movement has been updated)
    sendUDP(theGame, udpData, udpValues, &flag, &flagSendOnStartup);

    //receive udp data
    receiveUDP(theGame, udpData, udpValues);


}

PUBLIC UDPData UDPDataReset()
{
    UDPData u;
    u.x = 0;
    u.y = 0;
    u.status = 0;
    u.playerID = 0;
    u.moveDirection = '0';
    u.placeBomb = 0;
    u.noOfLives = 3;

    for(int i; i < 4; i++)
        u.score[i] = 0;

    u.powerupsX = 0;
    u.powerupsY = 0;
    u.powerupsType = 0;
    u.powerupsID = 0;
    return u;
}

PUBLIC UDPStruct createUDPstruct()
{
    UDPStruct u;
    strcpy(u.serverIp, "127.0.0.1");
    return u;
}

PUBLIC void pingUDPserver(Game theGame, UDPData *udpData, UDPStruct *udpValues)
{
    memcpy(udpValues->p->data, &(*udpData), sizeof(UDPData) + 1);
    udpValues->p->len = sizeof(UDPData) + 1;
    udpValues->p->address.host = udpValues->srvadd.host; /* Set the destination host */
    udpValues->p->address.port = udpValues->srvadd.port; /* And destination port */
    SDLNet_UDP_Send(udpValues->sd, -1, udpValues->p);
}


//detta gör  vi via TCP istället
PUBLIC void getPlayerIDviaUDP(Game theGame, UDPData *udpData, UDPStruct *udpValues)
{
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
    setLocalID(theGame, udpData->playerID);
    printf("Player ID: %d\n", theGame->playerIDLocal);
}

//detta gör vi via TCP istället
PUBLIC void checkPlayerAmmount(Game theGame)
{
    // detta ska ändras via servern sen.
    theGame->playerAmount = PLAYERAMOUNT;
}

PUBLIC void initSDLNet()
{
    // initiera SDL NET
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
}
PUBLIC void initUDP(UDPStruct *u)
{
    if (!(u->sd = SDLNet_UDP_Open(0)))  
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    /* Resolve server name  */
    if (SDLNet_ResolveHost(&u->srvadd, u->serverIp, 2001) == -1)
    {
        fprintf(stderr, "SDLNet_ResolveHost(%s : 2001): %s\n", u->serverIp, SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    if (!((u->p = SDLNet_AllocPacket(sizeof(UDPData) + 1)) && (u->p2 = SDLNet_AllocPacket(sizeof(UDPData) + 1))))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

}
void UDPSetPlayerID(UDPData *u, int id)
{
    u->playerID = id;
}
void UDPSetXPos(UDPData *u, int x)
{
    u->x = x;
}
void UDPSetYPos(UDPData *u, int y)
{
    u->y = y;
}
void UDPSetScore(UDPData *u, int id, int score)
{
    u->score[id] = score;
}
void UDPSetNoOfLives(UDPData *u, int lives)
{
    u->noOfLives = lives;
}
void UDPSetMoveDirection(UDPData *u, char c)
{
    u->moveDirection = c;
}

int UDPGetPlayerID(UDPData *u)
{
    return u->playerID;
}
int UDPGetScore(UDPData *u, int i)
{
    return u->score[i];
}