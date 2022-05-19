#include  "udpServerWithTCPclient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL2/SDL_net.h"
#include "SDL2/SDL_thread.h"
#define PUBLIC /* empty */
#define PRIVATE static

PUBLIC TCPstruct createTCPstruct()
{
    TCPstruct u;
    return u;
}
  
PUBLIC void initSDLNet()
{
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
}

PUBLIC void initTCP(TCPstruct *u, char serverIP[])
{
	/* Resolve the host we are connecting to */
	if (SDLNet_ResolveHost(&u->ip, serverIP, 2000) < 0)
	{
		fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Open a connection with the IP provided (listen on the host's port) */
	if (!(u->sd = SDLNet_TCP_Open(&u->ip)))
	{
		fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
}

PUBLIC void closeTCP(TCPstruct *u)
{
	SDLNet_TCP_Close(u->sd);
}