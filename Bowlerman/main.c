#include <stdio.h>
#include <SDL2/SDL.h>
#include "game.h"

int main(int argc, char *argv[])
{ 
    Game bowlerman = createGame();
    loadMedia(bowlerman, "grass00.bmp");
    gameUpdate(bowlerman);
    destroyGame(bowlerman);
    return 0;
}