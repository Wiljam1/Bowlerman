#include <stdio.h>
#include "game.h"

int main(int argc, char *argv[])
{ 
    Game bowlerman = createWindow();
    gameUpdate(bowlerman);
    destroyGame(bowlerman);
    printf("The End of main!\n");
    return 0;
}