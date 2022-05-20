#ifndef udpServerWithTCPclient_h
#define udpServerWithTCPclient_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//values for TCP, like IP-adress etc.
struct tcp_struct
{
    IPaddress ip;		/* Server address */
    TCPsocket sd;		/* Socket descriptor */  
};
typedef struct tcp_struct TCPstruct;


//values returned to UDP-server
struct buffer3
{
	int playerAmmount;
	Uint32 IPclient[5]; 
    Uint32 portClient[5]; 
};
typedef struct buffer3 Buffer3;
Buffer3 buffer3;

TCPstruct createTCPstruct();
void initSDLNet();
void initTCP(TCPstruct *u, char serverIP[]);
void closeTCP(TCPstruct *u);

#endif