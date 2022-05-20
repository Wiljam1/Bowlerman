#if 0
#!/bin/sh
gcc -Wall `sdl-config --cflags` tcps.c -o tcps `sdl-config --libs` -lSDL_net
 
exit
#endif

//tcp server med globala variabler och ger värden tillbaka till klient
//ingen UDP server är connectad till denna server.
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL2/SDL_thread.h"
#include "SDL2/SDL_net.h"

int playerAmmount=0; 		//hur många klienter är inne just nu
#define MAXPLAYERS 5 		//max antal spelare tillåtna på servern
TCPsocket csd[MAXPLAYERS+2]; /* Client socket descriptor. En extra socket för UDP servern */
SDL_cond* gWakeUpMain = NULL;    //condition för att väcka main-loopen. Just nu en NULL-pointer
SDL_mutex* gMainLock = NULL;   //The protective mutex
SDL_Thread* thread[MAXPLAYERS+2];   //trådar

//send to regular client players
struct buffer2
{
	int playerAmmount;  //how many clients are connected
	int playerID;
	int startflag;   //startflag is used to indicate whether game should start or not
};
typedef struct buffer2 Buffer2;

//send to UDP server
struct buffer3
{
	int playerAmmount;
	Uint32 IPclient[5]; 
    Uint32 portClient[5]; 
};
typedef struct buffer3 Buffer3;
Buffer3 buffer3;


//sends data via TCP:
//socketID is what socket-descriptor to send to.
//buffer is what data is to be sent
//len is the length of the buffer
//this function is not used.
void sendTCP(int socketID, char * buffer, int len)
{
	if (SDLNet_TCP_Send(csd[socketID], (void *)buffer, len) < len)
	{
		fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
}

//send buffer 2 (to players)
void sendTCPstruct2(int socketID, Buffer2 *buffer, int len)
{
	if (SDLNet_TCP_Send(csd[socketID], (void *)buffer, len) < len)
	{
		fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
}

//send buffer3 (to UDP server)
void sendTCPstruct3(int socketID, Buffer3 *buffer, int len)
{
	if (SDLNet_TCP_Send(csd[socketID], (void *)buffer, len) < len)
	{
		fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
}


//the function each thread enters:
//receives and handles TCP requests
int threadTCPReceive(void * data)
{
	SDL_LockMutex( gMainLock ); //Lock
	//vet inte varför jag måste konvertera på detta sätt:
	int *test= (int*) &data;
	int threadID = *test;
	int playerID = playerAmmount; // sätt -1 här eftersom UDP servern också kommer connect:a
	SDL_UnlockMutex( gMainLock );	//Unlock
    SDL_CondSignal( gWakeUpMain ); //wake up main thread here. Signal producer. väcker main efter att ha läst av threadID värdet (playerammount)
	
	//jag måste oxå förstöra trådarna när de är klara. vid "quit"

	printf("threadID: %d woken up\n", threadID);

	char buffer[512]; //måste man använda malloc här för att trådarna inte ska använda sig av buffern?
						//glöm isf inte att free:a när tråden stängs
	
	//behövs malloc här tror jag. sen free malloc i slutet på loopen.
	Buffer2 buffer2;
	buffer2.playerAmmount=0;
	buffer2.playerID=playerID;
	buffer2.startflag=0;

	int quit=0;

	while(!quit)
	{
		if (SDLNet_TCP_Recv(csd[threadID], buffer, 512) > 0) //Recv och send är blockerande, därför måste man först kolla om socket är redo med socketReady
		{
			printf("Client %d says: %s\n", threadID, buffer);  //put playerammounts here
			int len = sizeof(Buffer2) + 1;
			buffer2.playerAmmount=playerAmmount; //sätt -1 här om UDP-servern ska vara med

			if(strcmp(buffer, "exit") == 0)	/* Terminate this connection */
			{
				/* Close the client socket */
				printf("Terminate connection\n");
				SDLNet_TCP_Close(csd[threadID]);
				SDL_WaitThread(thread[threadID], NULL );
				thread[threadID] = NULL;
				
				//quit = 1;

				//kommer ej skicka tillbaka data här eftersom csd stängs.
				//minska på counter här också och förskjut hela arrayen
			}
			else if(strcmp(buffer, "playerID") == 0 || strcmp(buffer, "playerAmount") == 0 )	/* Send back playerID and playerAmmount*/
			{
				sendTCPstruct2(threadID, &buffer2,len);
			}
			else if(strcmp(buffer, "start") == 0)	/* start game */
			{
				//sendTCPstruct3(0,  &buffer3,len); //send to UDP server
				
				buffer2.startflag=1;
				for(int i=0; i<playerAmmount;i++) //send startflag=1 to all players
				{
					sendTCPstruct2(i,  &buffer2,len); //+1 för UDP servern connectar först.
				}
			}
			else if(strcmp(buffer, "udpServer") == 0)	/* establish UDB server IP */
			{
				
			}
			else sendTCP(threadID, buffer, len);  //send info back
			
		}	
	}
	return 0;
}
 
int main(int argc, char **argv)
{

	TCPsocket sd; 		/* Socket descriptor */
	IPaddress ip, *remoteIP;
	Uint32 ipaddr;
	int quit=0;
	
    gWakeUpMain = SDL_CreateCond();  //Create conditions för att väcka main-loopen.
    gMainLock = SDL_CreateMutex();  //Create the mutex

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
 
	printf("Server started\n");
	/* Wait for a connection, send data and term */
	while (!quit)
	{
		/* This check the sd if there is a pending connection.
		* If there is one, accept that, and open a new socket for communicating */
		if (csd[playerAmmount] = SDLNet_TCP_Accept(sd))
		{
			/* Now we can communicate with the client using csd socket
			* sd will remain opened waiting other connections */

			//printf("inside TCP accept, with client socket: %d \n", csd[playerAmmount]);
			/* Get the remote address */
			if (remoteIP = SDLNet_TCP_GetPeerAddress(csd[playerAmmount]))
			{
				/* Print the address, converting in the host format */
				//printf("Host connected: %x %d\n", SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
				/* print out the clients IP and port number */
				ipaddr=SDL_SwapBE32(remoteIP->host);
				//printf("ip string: %x port string: %d\n", SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
				printf("Accepted a connection from %d.%d.%d.%d port %hu\n",
					ipaddr>>24,
					(ipaddr>>16)&0xff,
					(ipaddr>>8)&0xff,
					ipaddr&0xff,
					remoteIP->port);
				
				//buffer3 is used to send data about  clients to UDP-server
				buffer3.playerAmmount=playerAmmount;
				buffer3.IPclient[playerAmmount]=remoteIP->host;  //save IP adress of connected client in struct
				buffer3.portClient[playerAmmount]=remoteIP->port; //save IP port of connected client in struct
				
    			SDL_LockMutex( gMainLock ); //Lock
				thread[playerAmmount]= SDL_CreateThread(threadTCPReceive, "threadForServer", (void*) playerAmmount );
				SDL_CondWait(gWakeUpMain, gMainLock); //make main fall asleep. Detta pga att det annars blir trådosäkert med variabeln playerAmmount.
    			SDL_UnlockMutex( gMainLock );	//Unlock
				playerAmmount++;
			}
			else
				fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
		}
	}

	//Remove timer in case the call back was not called
	for(int i=0; i<MAXPLAYERS+2; i++)
	{
		thread[i] = NULL;  //remove thread.
	}

	//Destroy conditions
    SDL_DestroyCond(gWakeUpMain);
    gWakeUpMain = NULL;

	//Destroy the mutex
    SDL_DestroyMutex(gMainLock);
    gMainLock = NULL;

	SDLNet_TCP_Close(sd);
	SDLNet_Quit();
 
	return EXIT_SUCCESS;
} 
