#if 0
#!/bin/sh
gcc -Wall `sdl-config --cflags` udps.c -o udps `sdl-config --libs` -lSDL_net
 
exit
#endif
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "SDL2/SDL_net.h"
 
int main(int argc, char **argv)
{
	UDPsocket sd;       /* Socket descriptor */
	UDPpacket *pRecive;       /* Pointer to packet memory */
	UDPpacket *pSent;
    Uint32 IPclient1=0; 
    Uint32 IPclient2=0;
    Uint32 portClient1=0; 
    Uint32 portClient2=0;
    int quit, a, b;
 
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
            if(IPclient1 == 0 && portClient1 == 0){
                printf("Client 1\n");
                IPclient1 = pRecive->address.host;
                portClient1 = pRecive->address.port;
            }else if(pRecive->address.port != portClient1  && IPclient2 == 0){
                printf("Client 2\n");
                IPclient2 = pRecive->address.host;
                portClient2 = pRecive->address.port;
            }else{
                if (pRecive->address.port == portClient1){
                    printf("Recived data\n");
                    if(IPclient2 != 0){
                        printf("Send to Client 2\n");
                        pSent->address.host = IPclient2;	/* Set the destination host */
		                pSent->address.port = portClient2;
                        sscanf((char * )pRecive->data, "%d %d\n", &a, &b);
                        printf("%d %d\n", a, b);
                        sprintf((char *)pSent->data, "%d %d\n", a,  b);
                        pSent->len = strlen((char *)pSent->data) + 1;
                        SDLNet_UDP_Send(sd, -1, pSent);
                    }
                } else if (pRecive->address.port == portClient2){
                    printf("Send to Client 1\n");    
                    pSent->address.host = IPclient1;	/* Set the destination host */
		            pSent->address.port = portClient1;
                    sscanf((char * )pRecive->data, "%d %d\n", &a, &b);
                    printf("%d %d\n", a, b);
                    sprintf((char *)pSent->data, "%d %d\n", a, b);
                    pSent->len = strlen((char *)pSent->data) + 1;
                    SDLNet_UDP_Send(sd, -1, pSent);
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
