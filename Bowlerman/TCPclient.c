#if 0
#!/bin/sh
gcc -Wall `sdl-config --cflags` tcpc.c -o tcpc `sdl-config --libs` -lSDL_net
 
exit
#endif
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL2/SDL_net.h"
#include "tcpClient.h"
#define PUBLIC /* empty */
#define PRIVATE static


PUBLIC TCPstruct createTCPstruct()
{
    TCPstruct u;
    return u;
}
  
PUBLIC void initTCP(TCPstruct *u)
{
	/* Resolve the host we are connecting to */
	if (SDLNet_ResolveHost(&u->ip, "127.0.0.1", 2000) < 0)
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
PUBLIC void manageTCP(TCPstruct *u)
{
	int quit, len;
	char buffer[512];
 
	//kanske skapa tråd här och sen stäng tråden efter menysystemet e klart.

	/* Send messages */
	quit = 0;
	while (!quit)
	{
		printf("Write something:\n>");
		scanf("%s", buffer);

		//info sent to server
		len = strlen(buffer) + 1;
		if (SDLNet_TCP_Send(u->sd, (void *)buffer, len) < len)
		{
			fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		}

		if(strcmp(buffer, "exit") == 0)
			quit = 1;
		if(strcmp(buffer, "quit") == 0)
			quit = 1;

		printf("info sent from client\n");

		//info received from server
		if (SDLNet_TCP_Recv(u->sd, buffer, 512) > 0)
		{
			printf("Received from server: %s\n", buffer);
		}
		
	}
} 

