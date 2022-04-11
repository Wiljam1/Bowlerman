#include <stdio.h>
#include <SDL2/SDL.h>
#include "game.h"

int main(int argc, char *argv[])
{ 
    Game bowlerman = createGame();
    SDL_Delay(20000);
    destoryGame(bowlerman);
    return 0;
}