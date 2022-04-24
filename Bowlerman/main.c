#include <stdio.h>
#include <SDL2/SDL.h>
#include "game.h"

int main(int argc, char *argv[])
{ 
    Game bowlerman = createGame();
    gameUpdate(bowlerman);
    destroyGame(bowlerman);
    return 0;
}