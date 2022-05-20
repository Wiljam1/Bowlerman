#if 0
#!/bin/sh
gcc -Wall `sdl-config --cflags` tcpc.c -o tcpc `sdl-config --libs` -lSDL_net
 
exit
#endif
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL2/SDL_net.h"
#include "SDL2/SDL_thread.h"
#include "tcpClient.h"
#define PUBLIC /* empty */
#define PRIVATE static



PUBLIC TCPstruct createTCPstruct()
{
    TCPstruct u;
    return u;
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

PUBLIC int receiveTCP(void * data)
{

	//TCPstruct *u = (TCPstruct *) data;
	//char buffer[512];

	ThreadStruct *threadStruct = (ThreadStruct *) data;
	char buffer[512];

	int quit =0;
	while(!quit)
	{
		//info received from server
		if (SDLNet_TCP_Recv(threadStruct->sd, buffer, 512) > 0)
		{
			//*buffer = (Buffer2 *) buffer; //cast buffer to a struct
			//buffer2= (Buffer2 *) buffer;
			Buffer2 *bufferTemp = (Buffer2 *) buffer;
			printf("received buffer playerID: %d\n", bufferTemp->playerID);
			printf("received buffer playerAmmount: %d\n", bufferTemp->playerAmmount);
			printf("received buffer startFlag: %d\n", bufferTemp->startflag);

			threadStruct->playerAmmount=bufferTemp->playerAmmount;
			//threadStruct->playerID=bufferTemp->playerID;
			threadStruct->startflag=bufferTemp->startflag;
			if (threadStruct->startflag==1)
			{
				//printf("inside startflag=1 in thread\n");
				quit=1;
			}


			//printf("Received from server string: %s\n", buffer);
			//printf("Received from server int: %d\n", buffer[0]);  //detta är korrekt
		}	
	}
	
	return 0;
}

PRIVATE void sendTCPtoServer(TCPstruct *u, char * buffer, int len)
{
	if (SDLNet_TCP_Send(u->sd, (void *)buffer, len) < len)
	{
		fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
}

PUBLIC int startGameViaTCP(TCPstruct *u, ThreadStruct * threadStruct)
{
	char buffer[512] = {"start"};
	int len = strlen(buffer) + 1;

	//sends TCP 
	sendTCPtoServer(u, buffer, len);

	//receives TCP
	if (SDLNet_TCP_Recv(u->sd, buffer, 512) > 0)
	{
		Buffer2 *bufferTemp = (Buffer2 *) buffer;		//vi vill ha en temp-buffert eftersom playerID ändras 
		threadStruct->playerAmmount= bufferTemp->playerAmmount;
		threadStruct->startflag=bufferTemp->startflag;
	}
	else
	{
		printf("Error: could not receive TCP from server\n");
	}
	return threadStruct->playerAmmount;
}

PUBLIC int getPlayerIDviaTCP(TCPstruct *u)
{
	char buffer[512] = {"playerID"};
	int len = strlen(buffer) + 1;
	int playerID;

	//sends TCP 
	sendTCPtoServer(u, buffer, len);

	//receive TCP
	if (SDLNet_TCP_Recv(u->sd, buffer, 512) > 0)
	{
		Buffer2 *bufferTemp = (Buffer2 *) buffer;		//vi vill ha en temp-buffert eftersom playerID ändras 
		playerID = bufferTemp->playerID;
	}

	return playerID;
}

PUBLIC void manageTCP(TCPstruct *u)
{
	int quit, len;
	char buffer[512];
	
	ThreadStruct threadStruct;
	threadStruct.sd=u->sd;
	threadStruct.startflag=0;
	threadStruct.playerID=-1;

	//Buffer2 buffer2;
	//buffer2.playerAmmount=-5;
	//buffer2.playerID=-5;
	//buffer2.startflag=-5;

 	SDL_Thread* thread = SDL_CreateThread(receiveTCP, "test", (void *) &threadStruct);

	
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
			printf("error in main\n");
			fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		}

		if(threadStruct.playerID != -1)
		{
			//return playerID;
		}

		if(threadStruct.startflag==1)
		{
			printf("inside destroy thread in main\n");
			//Remove timer in case the call back was not called
            SDL_WaitThread( thread, NULL );
			thread=NULL;
			//quit=1;

			//theGame->playerID = threadStruct.playerID;
			//thegame->playerammount= threadStruct.playerAmmount;
			//theGame->
		} 


		//SDL_Delay(1000);  //
		printf("playerID in main: %d\n", threadStruct.playerID);
		printf("playerAmount in main: %d\n", threadStruct.playerAmmount);
		printf("startflag in main: %d\n", threadStruct.startflag);
		

		//if(strcmp(buffer, "exit") == 0)
		//	quit = 1;
		//if(strcmp(buffer, "quit") == 0)
		//	quit = 1;

		//info received from server
		//if (SDLNet_TCP_Recv(u->sd, buffer, 512) > 0)
		//{
		//	//*buffer = (Buffer2 *) buffer; //cast buffer to a struct
		//	//buffer2= (Buffer2 *) buffer;
		//	Buffer2 *bufferTemp = (Buffer2 *) buffer;
		//	printf("received buffer playerID: %d\n", bufferTemp->playerID);
		//	printf("received buffer playerAmmount: %d\n", bufferTemp->playerAmmount);
		//	printf("received buffer startFlag: %d\n", bufferTemp->startflag);
		//	//printf("Received from server string: %s\n", buffer);
		//	//printf("Received from server int: %d\n", buffer[0]);  //detta är korrekt
		//}
		

		printf("info sent from client\n");


		
		
	}
} 

// PUBLIC void initSDLNet()
// {
// 	if (SDLNet_Init() < 0)
// 	{
// 		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
// 		exit(EXIT_FAILURE);
// 	}
// }

// int main(int argc, char *argv[])
// {
// 	initSDLNet();
	
// 	TCPstruct tcpValues = createTCPstruct();     //returns a struct for tcp-init-struct.	
// 	initTCP(&tcpValues, "127.0.0.1");		//connectar till angiven Ip-adress
// 	int playerID = getPlayerIDviaTCP(&tcpValues);  //detta ger en tråd-läcka. en tråd är lös.
// 	printf("playerID in main main %d\n", playerID);

// 	ThreadStruct threadStruct;
// 	threadStruct.sd=tcpValues.sd;
// 	threadStruct.startflag=0;
// 	SDL_Thread* thread = SDL_CreateThread(receiveTCP, "test", (void *) &threadStruct);
// 	//SDL_WaitThread(thread); //Elr en annan funktion som stänger ned tråden. Men kanske i slutet då.

// 	if(playerID==2)
// 	{
// 		startGameViaTCP(&tcpValues, &threadStruct);
// 		//SDL_terminateThread;
// 	}
	
// 	//SDL_Delay(1000); //bara tillfälligt

	
// 	//manageTCP(&tcpValues);					//skickar och tar emot data via TCP
// 	while(1)
// 	{
// 		if(threadStruct.startflag==1)
// 		{
// 			printf("playerammount: %d\n", threadStruct.playerAmmount);
// 			printf("startflag: %d\n", threadStruct.startflag);
// 			//get playerammount & start game (exit loop)
// 			thread=NULL;  //closes thread
// 			break;
// 		}
// 	}

// 	closeTCP(&tcpValues);

// 	SDLNet_Quit();
// }
