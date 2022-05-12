#ifndef udpClient_h
#define udpClient_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"


// data sent via UDP
struct data
{ 
    int x;
    int y;
    int status;
    int playerID;
    char moveDirection;
    int placeBomb;

    int powerupsX;
    int powerupsY;
    int powerupsType;
    int powerupsID;
};
typedef struct data UDPData;

//values for UDP, like IP-adress etc.
struct udpStruct
{
    UDPsocket sd;
    IPaddress srvadd;
    UDPpacket *p;
    UDPpacket *p2;   
};
typedef struct udpStruct UDPStruct;


void initSDLNet();          //initiates SDLNet
void initUDP(UDPStruct *u);   //inits SDL-net with correct IP-adresses etc.
UDPData UDPDataReset();  //creates and resets a UDP-data struct
UDPStruct createUDPstruct();     //initializes UDP IP-adress struct
void getPlayerIDviaUDP(Game theGame, UDPData *udpData, UDPStruct *udpValues);  //get player ID via UDP

void checkPlayerAmmount(Game theGame);  //checks how many players are online and  stores it in theGame->playerAmmount - currently the value is static and set at 4.
void manageUDP(Game theGame, UDPData *udpData, UDPStruct *udpValues);  //sends and receives data via UDP

#endif