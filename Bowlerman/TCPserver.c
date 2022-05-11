#if 0
#!/bin/sh
gcc -Wall `sdl-config --cflags` tcps.c -o tcps `sdl-config --libs` -lSDL_net
 
exit
#endif
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL2/SDL_thread.h"
#include "SDL2/SDL_net.h"

int playerAmmount=0; 		//hur många klienter är inne just nu
#define MAXPLAYERS 5 		//max antal spelare tillåtna på servern
TCPsocket csd[MAXPLAYERS+2]; /* Client socket descriptor. En extra socket för UDP servern */
SDL_cond* gWakeUpMain = NULL;    //condition för att väcka main-loopen. Just nu en NULL-pointer

int threadTCPReceive(void * data)
{
	//vet inte varför jag måste konvertera på detta sätt:
	int *test= (int*) &data;
	int threadID = *test;
    SDL_CondSignal( gWakeUpMain ); //wake up main thread here. Signal producer
	//väcker main efter att ha läst av threadID värdet (playerammount)

	printf("threadID: %d woken up\n", threadID);

	char buffer[512];
	int quit=0;

	while(!quit)
	{
		if (SDLNet_TCP_Recv(csd[threadID], buffer, 512) > 0) //Recv och send är blockerande, därför måste man först kolla om socket är redo med socketReady
		{
			printf("Client %d says: %s\n", threadID, buffer);  //put playerammounts here

			if(strcmp(buffer, "exit") == 0)	/* Terminate this connection */
			{
				/* Close the client socket */
				SDLNet_TCP_Close(csd[threadID]);
				printf("Terminate connection\n");
				//kommer ej skicka tillbaka data här eftersom csd stängs.
				//minska på counter här också och förskjut hela arrayen
			}
			if(strcmp(buffer, "quit") == 0)	/* Quit the program */
			{
				quit = 1;
				printf("Quit program\n");
			}

			//send info back
			int len = strlen(buffer) + 1;
			if (SDLNet_TCP_Send(csd[threadID], (void *)buffer, len) < len)
			{
				fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
				exit(EXIT_FAILURE);
			}
		}	
	}
	return 0;
}
 
int main(int argc, char **argv)
{

	TCPsocket sd; 		/* Socket descriptor */
	IPaddress ip, *remoteIP;
	int quit=0;
	SDL_Thread* threadID[MAXPLAYERS+2];   //trådar
    gWakeUpMain = SDL_CreateCond();  //Create conditions för att väcka main-loopen.

	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Resolving the host using NULL make network interface to listen */
	if (SDLNet_ResolveHost(&ip, NULL, 2000) < 0)
	{
		fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Open a connection with the IP provided (listen on the host's port) */
	if (!(sd = SDLNet_TCP_Open(&ip)))
	{
		fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Wait for a connection, send data and term */
	while (!quit)
	{
		/* This check the sd if there is a pending connection.
		* If there is one, accept that, and open a new socket for communicating */
		if (csd[playerAmmount] = SDLNet_TCP_Accept(sd))
		{
			/* Now we can communicate with the client using csd socket
			* sd will remain opened waiting other connections */

			printf("inside TCP accept, with client socket: %d \n", csd[playerAmmount]);
			/* Get the remote address */
			if (remoteIP = SDLNet_TCP_GetPeerAddress(csd[playerAmmount]))
			{
				/* Print the address, converting in the host format */
				printf("Host connected: %x %d\n", SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
				//threadTCPReceive(&playerAmmount);
				threadID[playerAmmount]= SDL_CreateThread(threadTCPReceive, "threadForServer", (void*) playerAmmount );
				
				SDL_CondWait(gWakeUpMain, NULL); //make main fall asleep. Detta pga att det annars blir trådosäkert med variabeln playerAmmount.
				playerAmmount++;
			}
			else
				fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
		}
	}

	//Remove timer in case the call back was not called
	for(int i=0; i<MAXPLAYERS+2; i++)
	{
    	SDL_WaitThread(threadID[i], NULL );
		threadID[i] = NULL;  //Nu pekar de på ingenting. Tänker att det är bra
	}

	//Destroy conditions
    SDL_DestroyCond(gWakeUpMain);
    gWakeUpMain = NULL;

	SDLNet_TCP_Close(sd);
	SDLNet_Quit();
 
	return EXIT_SUCCESS;
} 
