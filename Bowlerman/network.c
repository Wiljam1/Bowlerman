#include "network.h"
#include "bomb.h"
#define PUBLIC /* empty */
#define PRIVATE static



PUBLIC void manageUDP(Game theGame, UDPData *udpData, UDPInit *udpValues)
{
    static int flag=0;
    static int flag2=0;

    udpData->moveDirection = theGame->player[theGame->playerIDLocal].moveDirection;
    int x_posOld = theGame->player[theGame->playerIDLocal].xPosOld;
    int y_posOld = theGame->player[theGame->playerIDLocal].yPosOld;
    int x_pos = theGame->player[theGame->playerIDLocal].xPos;
    int y_pos = theGame->player[theGame->playerIDLocal].yPos;

    //check to see if we should send bomb with UDP
    static int bombUDPtimer=0;
    udpData->placeBomb=0;
    if(theGame->bombs[theGame->playerIDLocal].timerinit==1){  
        if(bombUDPtimer<1)    //vi ser till att skicka 1 packets, för om vi skickar för mycket blir det buggigt pga delay med UDP i slutet
        {
            udpData->placeBomb=1;
            bombUDPtimer++;
        }
        else {
            udpData->placeBomb=0;
        }
    }
    else{
        bombUDPtimer=0;
        udpData->placeBomb=0;
    }

        //flagga för att reset:a movement direction när spelaren står stilla.
    if( udpData->moveDirection != '0'){
       flag2=1;
    }
    if( (udpData->moveDirection == '0') && flag2){
       flag=1;
       flag2=0;
    }

    // send data if movement or bomb-placement
    //problem med flag. det gör att det laggar
    if (abs(x_posOld - x_pos)>=5 || abs(y_posOld - y_pos)>=5 || flag == 1 || udpData->placeBomb==1)
    {
        printf("%d %d\n", (int)x_pos, (int)y_pos);
        udpData->playerID = theGame->playerIDLocal;
        udpData->x = x_pos;
        udpData->y = y_pos;
        memcpy(udpValues->p->data, &(*udpData), sizeof(UDPData) + 1);
        // fwrite(&udpData, sizeof(struct data), 1, p->data);
        udpValues->p->len = sizeof(UDPData) + 1;
        // sprintf((char *)p->data, "%d %d\n", (int) x_pos, (int) y_pos);
        udpValues->p->address.host = udpValues->srvadd.host; /* Set the destination host */
        udpValues->p->address.port = udpValues->srvadd.port; /* And destination port */
        // p->len = strlen((char *)p->data) + 1;
        SDLNet_UDP_Send(udpValues->sd, -1, udpValues->p);
        theGame->player[theGame->playerIDLocal].xPosOld = x_pos; 
        theGame->player[theGame->playerIDLocal].yPosOld = y_pos;
        flag=0;
    }

    // receive data
    if (SDLNet_UDP_Recv(udpValues->sd, udpValues->p2))
    {
        int a, b;
        // sscanf((char * )p2->data, "%d %d\n", &a, &b);
        memcpy(&(*udpData), (char *)udpValues->p2->data, sizeof(struct data));
        int playerID = udpData->playerID;
        theGame->player[playerID].xPos = udpData->x;
        theGame->player[playerID].yPos = udpData->y;
        if (udpData->placeBomb==1){ 
            tryToPlaceBomb(theGame, playerID);
        }

        theGame->player[playerID].moveDirection = udpData->moveDirection;
        theGame->player[playerID].id = udpData->playerID;
        printf("UDP Packet incoming, x,y-coord: %d %d of player %d\n", udpData->x, udpData->y, udpData->playerID);
    }
}

PUBLIC UDPData UDPDataTransfer()
{
    UDPData u;
    u.x = 0;
    u.y = 0;
    u.status = 0;
    u.playerID = 0;
    u.moveDirection = '0';
    u.placeBomb = 0;
    return u;
}

PUBLIC UDPInit SetUDPValues()
{
    UDPInit u;
    return u;
}

PUBLIC void getPlayerIDviaUDP(Game theGame, UDPData *udpData, UDPInit *udpValues)
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
    printf("crash");
    theGame->playerIDLocal = udpData->playerID;
    printf("UDP Packet incoming %d\n", udpData->playerID);
    printf("%d", theGame->playerIDLocal);
    // detta ska ändras via servern sen.
    theGame->playerAmount = PLAYERAMOUNT;
}

PUBLIC void initSDLNet(UDPInit *u)
{
    // initiera SDL NET
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    if (!(u->sd = SDLNet_UDP_Open(0)))
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    /* Resolve server name  */
    if (SDLNet_ResolveHost(&u->srvadd, "127.0.0.1", 2000) == -1)
    {
        fprintf(stderr, "SDLNet_ResolveHost(192.0.0.1 2000): %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    if (!((u->p = SDLNet_AllocPacket(sizeof(UDPData) + 1)) && (u->p2 = SDLNet_AllocPacket(sizeof(UDPData) + 1))))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
}



