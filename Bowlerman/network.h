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


struct udp_init
{
    UDPsocket sd;
    IPaddress srvadd;
    UDPpacket *p;
    UDPpacket *p2;   
};
typedef struct udp_init UDPInit;



UDPData UDPDataTransfer();
UDPInit SetUDPValues();
void initSDLNet(UDPInit *u);
void manageUDP(Game theGame, UDPData *udpData, UDPInit *udpValues);

#endif