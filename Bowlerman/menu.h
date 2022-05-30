#ifndef menu_h
#define menu_h

#include "udpClient.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

#define PUBLIC /* empty */
#define PRIVATE static

void menu(Game theGame, bool *quitGame, UDPStruct *udpvalues); //runs the menu and checks for events
int updateMenu(); // changes menu tab and updates the buttons
void clearString(); // used to clear the ip string so that whenever the user can input, the ip the array is empty


#endif