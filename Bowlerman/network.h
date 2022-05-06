#ifndef network_h
#define network_h

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
};
typedef struct data UDPData;

//values for UDP, like IP-adress etc.
struct udp_init
{
    UDPsocket sd;
    IPaddress srvadd;
    UDPpacket *p;
    UDPpacket *p2;   
};
typedef struct udp_init UDPInit;



UDPData UDPDataTransfer();  //creates and resets a UDP-data struct
UDPInit SetUDPValues();     //initializes UDP IP-adress struct
void initSDLNet(UDPInit *u); //inits SDL-net with correct IP-adresses etc.
void getPlayerIDviaUDP(Game theGame, UDPData *udpData, UDPInit *udpValues);  //get player ID via UDP

void checkPlayerAmmount(Game theGame);  //checks how many players are online and  stores it in theGame->playerAmmount - currently the value is static and set at 4.
void manageUDP(Game theGame, UDPData *udpData, UDPInit *udpValues);  //sends and receives data via UDP

#endif