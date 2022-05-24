#include <stdio.h>
#include <SDL2/SDL.h>
#include "game.h"
#include "player.h"
#include "collisionDetection.h"

int main(int argc, char *argv[])
{ 
    Game bowlerman = createWindow();
    gameUpdate(bowlerman);
    destroyGame(bowlerman);
    printf("Crash ");
    return 0;
}