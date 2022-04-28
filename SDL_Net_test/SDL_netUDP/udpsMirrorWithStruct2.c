#if 0
#!/bin/sh
gcc -Wall `sdl-config --cflags` udps.c -o udps `sdl-config --libs` -lSDL_net
 
exit
#endif
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "SDL2/SDL_net.h"

struct data {
   int x;
   int y;
   int status;
};


 
int main(int argc, char **argv)
{
	UDPsocket sd;       /* Socket descriptor */
	UDPpacket *pRecive;       /* Pointer to packet memory */
	UDPpacket *pSent;
    Uint32 IPclient[3]={0}; 
    Uint32 portClient[3]={0}; 
    int quit, a, b;
    struct data udpData = {0, 0, 0};
 
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
	if (!((pSent = SDLNet_AllocPacket(512))&&(pRecive = SDLNet_AllocPacket(512))))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Main loop */
	quit = 0;
	while (!quit)
	{
		/* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
		if (SDLNet_UDP_Recv(sd, pRecive))
		{
			printf("UDP Packet incoming\n");
			printf("\tData:    %s\n", (char *)pRecive->data);
			printf("\tAddress: %x %x\n", pRecive->address.host, pRecive->address.port);
            for(int i=0; i<2; i++)
            {
                if(IPclient[i] == 0 && portClient[i] == 0){
                    printf("Client %d\n", i);
                    IPclient[i] = pRecive->address.host;
                    portClient[i] = pRecive->address.port;  
                }
                else if (pRecive->address.port == portClient[i]){
                    printf("Recived data\n");
                    for (int j=0; j<i; j++)
                    {
                        if(IPclient[j] != 0){
                            printf("Send to Client %d\n", j);
                            pSent->address.host = IPclient[j];	/* Set the destination host */
                            pSent->address.port = portClient[j];
                            //sscanf((char * )pRecive->data, "%d %d\n", &a, &b);
                            //printf("%d %d\n", a, b);
                            //sprintf((char *)pSent->data, "%d %d\n", a,  b);
                            memcpy(&udpData, (char * ) pRecive->data, sizeof(struct data));
                            printf("UDP Packet data %d %d\n", udpData.x, udpData.y);
                            memcpy((char *)pSent->data, &udpData , sizeof(struct data)+1);
                            pSent->len = sizeof(struct data)+1;
                            SDLNet_UDP_Send(sd, -1, pSent); 
                        }
                    }

                    for (int j=i; j<2; j++)
                    {
                        if(IPclient[j] != 0){
                            printf("Send to Client %d\n", j);
                            pSent->address.host = IPclient[j];	/* Set the destination host */
                            pSent->address.port = portClient[j];
                            //sscanf((char * )pRecive->data, "%d %d\n", &a, &b);
                            //printf("%d %d\n", a, b);
                            //sprintf((char *)pSent->data, "%d %d\n", a,  b);
                            memcpy(&udpData, (char * ) pRecive->data, sizeof(struct data));
                            printf("UDP Packet data %d %d\n", udpData.x, udpData.y);
                            memcpy((char *)pSent->data, &udpData , sizeof(struct data)+1);
                            pSent->len = sizeof(struct data)+1;
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
