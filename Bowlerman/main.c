#include <stdio.h>
#include <SDL2/SDL.h>
#include "game.h"
#include "player.h"
#include "collissionDetection.h"

int main(int argc, char *argv[])
{ 
    Game bowlerman = initializeGameWindow();
    gameUpdate(bowlerman);
    destroyGame(bowlerman);
    return 0;
}