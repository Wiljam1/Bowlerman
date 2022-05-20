#ifndef menu_h
#define menu_h

#include "udpClient.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

#define PUBLIC /* empty */
#define PRIVATE static

void changeMenu();
void menu(Game theGame, bool *quitGame, UDPStruct *udpvalues);
void checkClickEvent();
int updateMenu();
void join();
void clearString();


#endif