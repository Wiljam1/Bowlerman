
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
    int playerAmmount=2;
	while (!quit)
	{
		/* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
		if (SDLNet_UDP_Recv(sd, pRecive))
		{
			printf("UDP Packet incoming\n");
			printf("\tData:    %s\n", (char *)pRecive->data);
			printf("\tAddress: %x %x\n", pRecive->address.host, pRecive->address.port);
            
            //etablera vems IP-adress och port
            if(IPclient[0] == 0 && portClient[0] == 0){
                printf("Client 0\n");
                IPclient[0] = pRecive->address.host;
                portClient[0] = pRecive->address.port;
            }else if(pRecive->address.port != portClient[0] && IPclient[1] == 0){
                printf("Client 1\n");
                IPclient[1] = pRecive->address.host;
                portClient[1] = pRecive->address.port;
            }

            //skicka data 
            else
            {
                for(int i=0; i<playerAmmount; i++)
            {
                //skicka data till alla utom den IP som skickade
                if (pRecive->address.port == portClient[i]){
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

                    for (int j=i+1; j<playerAmmount; j++)
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