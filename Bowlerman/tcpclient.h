#ifndef tcpclient_h
#define tcpclient_h
#include "game.h"

//values for TCP, like IP-adress etc.
struct tcp_struct
{
    IPaddress ip;		/* Server address */
    TCPsocket sd;		/* Socket descriptor */  
};
typedef struct tcp_struct TCPstruct;

TCPstruct SetTCPValues();           //returns a struct for TCP
void initTCP(TCPstruct *u);             //initiate TCP. Här bör man kunna skicka in rätt IP att connecta till också
void manageTCP(TCPstruct *u);     //send and receive TCP
void closeTCP(TCPstruct *u);       //closes TCP

#endif