#include <stdio.h>
#include <SDL2/SDL.h>
#include "game.h"

int main(int argc, char *argv[])
{ 
    Game bowlerman = createGame();
    SDL_Delay(2000);        // Delay så fönstret inte stängs ner direkt.
    destoryGame(bowlerman);
    return 0;
}