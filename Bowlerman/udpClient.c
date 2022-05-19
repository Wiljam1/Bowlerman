#include "udpClient.h"
#include "bomb.h"
#include "powerup.h"
#define PUBLIC /* empty */
#define PRIVATE static
#define UPDATESPEED 1
#define MAXAMMOUNTOFBOMBS 5   //how many bombs are allowed to be placed by one player
void UDPSetScore(UDPData *u, int id, int score);
int UDPGetPlayerID(UDPData *u);
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

PRIVATE void sendUDP(Game theGame,UDPData *udpData, UDPStruct *udpValues, int *flag)
{
    int playerID = getLocalID(theGame);
    int x_posOld = playerGetOldXpos(theGame->player[playerID]); //theGame->player[playerID].xPosOld;
    int y_posOld = playerGetOldYPos(theGame->player[playerID]); //theGame->player[playerID].yPosOld;
    float x_pos = playerGetXPosition(theGame->player[playerID]); // theGame->player[playerID].xPos;
    float y_pos = playerGetYPosition(theGame->player[playerID]);  //theGame->player[playerID].yPos;
    //int noOfLives = getPlayerNoOfLives(theGame->player[playerID]); //theGame->player[playerID].noOfLives;
    static int oldScore = 0, scoreGUIFlag = 0;

    if(theGame->player[playerID].score != oldScore){
        oldScore = playerGetScore(theGame->player[playerID]);
        scoreGUIFlag = 1;
    }
    // send data if movement or bomb-placement
    if (abs(x_posOld - x_pos) >= UPDATESPEED || abs(y_posOld - y_pos) >= UPDATESPEED || *flag == 1 || udpData->placeBomb==1 || scoreGUIFlag == 1)
    {
        //printf("%d %d\n", (int)x_pos, (int)y_pos);
        udpData->playerID = playerID;
        udpData->x = x_pos;
        udpData->y = y_pos;
        udpData->powerupsX = 0;
        udpData->noOfLives = playerGetNoOfLives(theGame->player[playerID]);
        udpData->score[playerID] = playerGetScore(theGame->player[playerID]);
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
        // fwrite(&udpData, sizeof(struct data), 1, p->data);
        udpValues->p->len = sizeof(UDPData) + 1;
        // sprintf((char *)p->data, "%d %d\n", (int) x_pos, (int) y_pos);
        udpValues->p->address.host = udpValues->srvadd.host; /* Set the destination host */
        udpValues->p->address.port = udpValues->srvadd.port; /* And destination port */
        // p->len = strlen((char *)p->data) + 1;
        SDLNet_UDP_Send(udpValues->sd, -1, udpValues->p);
        playerSetOldXPos(&theGame->player[playerID], x_pos); 
        playerSetOldYPos(&theGame->player[playerID], y_pos);
        //theGame->player[playerID].xPosOld = x_pos;
        //theGame->player[playerID].yPosOld = y_pos;
        *flag=0;
        scoreGUIFlag = 0;
       // printf("x:%d y:%d dir:%c \n", udpData->x, udpData->y, udpData->moveDirection);
    }
}

PRIVATE void receiveUDP(Game theGame,UDPData *udpData, UDPStruct *udpValues)
{

    // receive data
    if (SDLNet_UDP_Recv(udpValues->sd, udpValues->p2))
    {
        int a, b;
        static int oldScore[4] = {0};
        // sscanf((char * )p2->data, "%d %d\n", &a, &b);
        memcpy(&(*udpData), (char *)udpValues->p2->data, sizeof(struct data));
        int playerID = UDPGetPlayerID(udpData); //int playerID = udpData->playerID;
        playerSetXPos(&(theGame->player[playerID]), udpData->x);
        playerSetYPos(&(theGame->player[playerID]), udpData->y);
        //theGame->player[playerID].xPos = (float)udpData->x;
        //theGame->player[playerID].yPos = (float)udpData->y;
        //printf("x:%d y:%d dir:%c ", udpData->x, udpData->y, udpData->moveDirection);
        if (udpData->placeBomb==1){ 
            tryToPlaceBomb(theGame, playerID);
        }
        
        if(udpData->powerupsX != 0)
        {
            theGame->powerups[udpData->powerupsID] = powerupPlace(udpData->powerupsX-WIDTH/119, udpData->powerupsY-WIDTH/119, udpData->powerupsType);
            theGame->powerups[udpData->powerupsID].indestructable = timerForPowerups(SDL_GetTicks(), 1500, udpData->powerupsID);
        }
        playerSetMoveDirection(&(theGame->player[playerID]), udpData->moveDirection);
        playerSetID(&(theGame->player[playerID]), udpData->playerID);
        playerSetNoOfLives(&(theGame->player[playerID]), udpData->noOfLives);
        playerSetScore(&(theGame->player[playerID]), udpData->score[playerID]);
        //theGame->player[playerID].id = udpData->playerID;
        //theGame->player[playerID].moveDirection = udpData->moveDirection;
        //theGame->player[playerID].noOfLives = udpData->noOfLives;
        //theGame->player[playerID].score = udpData->score[playerID];
        for(int i = 0; i < 4; i++){
            if(udpData->score[i] != oldScore[i]){
                flagSetUpdate(theGame, true); //theGame->updateFlag = true;
                oldScore[i] = udpData->score[i];
            }    
        }
        //printf("UDP Packet incoming, x,y-coord: %d %d of player %d, placebomb: %d\n", udpData->x, udpData->y, udpData->playerID, udpData->placeBomb);
    }
}

PUBLIC void manageUDP(Game theGame, UDPData *udpData, UDPStruct *udpValues)
{
    static int flag=0;
    static int flag2=0;
    
    int playerID = theGame->playerIDLocal;
    udpData->moveDirection = theGame->player[playerID].moveDirection;
    

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

    //send udp data (if e.g. movement has been updated)
    sendUDP(theGame, udpData, udpValues, &flag);

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
    u.noOfLives = 5;

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
    return u;
}

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
    theGame->playerIDLocal = udpData->playerID;
    //printf("UDP Packet incoming %d\n", udpData->playerID);
    printf("Player ID: %d\n", theGame->playerIDLocal);
    
}

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
    if (!(u->sd = SDLNet_UDP_Open(2001)))  
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    /* Resolve server name  */
    if (SDLNet_ResolveHost(&u->srvadd, u->serverIp, 2001) == -1)
    {
        fprintf(stderr, "SDLNet_ResolveHost(192.0.0.1 2001): %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    if (!((u->p = SDLNet_AllocPacket(sizeof(UDPData) + 1)) && (u->p2 = SDLNet_AllocPacket(sizeof(UDPData) + 1))))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

}

void UDPSetScore(UDPData *u, int id, int score)
{
    u->score[id] = score;
}
int UDPGetPlayerID(UDPData *u)
{
    return u->playerID;
}



