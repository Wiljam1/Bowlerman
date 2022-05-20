#ifndef tcpClient_h
#define tcpClient_h

#include "SDL2/SDL_net.h"


//values for TCP, like IP-adress etc.
struct tcp_struct
{
    IPaddress ip;		/* Server address */
    TCPsocket sd;		/* Socket descriptor */  
};
typedef struct tcp_struct TCPstruct;

struct buffer2
{
	int playerAmmount;  //how many players are currently online
	int playerID;
	int startflag; //determines whether the game should start or not
};
typedef struct buffer2 Buffer2;

//need this struct to have the buffer2 and socket-descriptor for a single pointer for the thread managing TCP-receive
struct threadStruct
{
    int playerAmmount;
	int playerID;
	int startflag;
    TCPsocket sd;
};
typedef struct threadStruct ThreadStruct;

//void initSDLNet();
int receiveTCP(void * data); //receives info whenever games starts. Sets playerAmount when starting.
int startGameViaTCP(TCPstruct *u, ThreadStruct * threadStruct); //sends start-request to TCP-server and sets playeramount.
int getPlayerIDviaTCP(TCPstruct *u); //returns playerID
TCPstruct createTCPstruct();           //returns a struct for TCP
void initTCP(TCPstruct *u, char serverIP[]);             //initiate TCP. Här bör man kunna skicka in rätt IP att connecta till också
void manageTCP(TCPstruct *u);     //send and receive TCP
void closeTCP(TCPstruct *u);       //closes TCP


#endif