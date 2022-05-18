
#if 0
#!/bin/sh
gcc -Wall `sdl-config --cflags` udps.c -o udps `sdl-config --libs` -lSDL_net
 
exit
#endif

//UDP server for infinite amount of players.
//limitations: 512 bytes of data per package (?)
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "SDL2/SDL_net.h"

struct data {
   int x;
   int y;
   int status;
   int playerID;
   int noOfLives;
   int score[4];
   char moveDirection;
    int placeBomb;
    int powerupsX;
    int powerupsY;
    int powerupsType;
    int powerupsID;
};

int main(int argc, char **argv)
{
	UDPsocket sd;       /* Socket descriptor */
	UDPpacket *pRecive;       /* Pointer to packet memory */
	UDPpacket *pSent;
    Uint32 IPclient[4]={0}; 
    Uint32 portClient[4]={0}; 
    int quit, a, b;
    struct data udpData = {0, 0, 0, 0};
 
	/* Initialize SDL_net */
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Open a socket */
	if (!(sd = SDLNet_UDP_Open(2000)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Make space for the packet */
    if (!((pSent = SDLNet_AllocPacket(sizeof(struct data)+1))&& (pRecive = SDLNet_AllocPacket(sizeof(struct data)+1))))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Main loop */
	quit = 0;
    int playerAmount=0;
	while (!quit)
	{
		/* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
		if (SDLNet_UDP_Recv(sd, pRecive))
		{
			printf("UDP Packet incoming\n");
			printf("\tData:    %s\n", (char *)pRecive->data);
			printf("\tAddress: %x %x\n", pRecive->address.host, pRecive->address.port);
            
            //etablera client 0's IP-adress och port. Detta bör man nog göra via TCP
            if(IPclient[0] == 0 && portClient[0] == 0){
                printf("Client 0\n");
                IPclient[0] = pRecive->address.host;
                portClient[0] = pRecive->address.port;
                playerAmount++;

                //send players ID:
                memcpy(&udpData, (char * ) pRecive->data, sizeof(struct data)); //detta behövs egentligen inte
                udpData.playerID=0;
                memcpy((char *)pSent->data, &udpData , sizeof(struct data)+1);
                pSent->len = sizeof(struct data)+1;
                pSent->address.host = IPclient[0];	/* Set the destination host */
                pSent->address.port = portClient[0];
                SDLNet_UDP_Send(sd, -1, pSent);
                printf("initializing client 0\n");
            }

            //etablera vems IP-adress och port.
            for (int i=0; i<playerAmount; i++)
            {
                if (pRecive->address.port == portClient[i]) break;
                else if (IPclient[i+1] == 0)
                {
                    printf("Client %d\n", i+1);
                    IPclient[i+1] = pRecive->address.host;
                    portClient[i+1] = pRecive->address.port;
                    

                    //send playerID to player: skicka via TCP istället
                    memcpy(&udpData, (char * ) pRecive->data, sizeof(struct data)); //detta behövs inte egentligen
                    udpData.playerID=playerAmount;
                    memcpy((char *)pSent->data, &udpData , sizeof(struct data)+1);
                    pSent->len = sizeof(struct data)+1;
                    pSent->address.host = IPclient[i+1];	/* Set the destination host */
                    pSent->address.port = portClient[i+1];
                    SDLNet_UDP_Send(sd, -1, pSent);
                    printf("initializing client %d\n", i+1);

                    playerAmount++;

                    break;
                }
            }


            //skicka data 
            for(int i=0; i<playerAmount; i++)
            {
                
                if (pRecive->address.port == portClient[i]){
                    printf("Recived data\n");

                    //copy data:
                    memcpy(&udpData, (char * ) pRecive->data, sizeof(struct data));
                    printf("UDP Packet data %d %d %c\n", udpData.x, udpData.y, udpData.moveDirection);
                    memcpy((char *)pSent->data, &udpData , sizeof(struct data)+1);
                    pSent->len = sizeof(struct data)+1;
                    //sscanf((char * )pRecive->data, "%d %d\n", &a, &b);
                    //printf("%d %d\n", a, b);
                    //sprintf((char *)pSent->data, "%d %d\n", a,  b);


                    //send data: skicka data till alla utom den IP som skickade
                    for (int j=0; j<i; j++)
                    {
                        if(IPclient[j] != 0){
                            printf("Send to Client %d\n", j);
                            pSent->address.host = IPclient[j];	/* Set the destination host */
                            pSent->address.port = portClient[j];
                            SDLNet_UDP_Send(sd, -1, pSent); 
                        }
                    }

                    //send data:
                    for (int j=i+1; j<playerAmount; j++)
                    {
                        if(IPclient[j] != 0){
                            printf("Send to Client %d\n", j);
                            pSent->address.host = IPclient[j];	/* Set the destination host */
                            pSent->address.port = portClient[j];
                            SDLNet_UDP_Send(sd, -1, pSent); 
                        }
                    }
                }
            }

			/* Quit if packet contains "quit" */
			if (strcmp((char *)pSent->data, "quit") == 0)
				quit = 1;
		}		
	}
 
	/* Clean and exit */
	SDLNet_FreePacket(pSent);
    SDLNet_FreePacket(pRecive);
	SDLNet_Quit();
	return EXIT_SUCCESS;
} 