#include <stdio.h>
#include <SDL2/SDL.h>
#include "game.h"
#include "player.h"
#include "collissionDetection.h"

int main(int argc, char *argv[])
{ 
    Game game = createWindow();
    gameUpdate(game);
    destroyGame(game);
    return 0;
}