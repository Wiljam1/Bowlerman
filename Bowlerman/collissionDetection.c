#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "player.h"
#include "collissionDetection.h"

const int WIDTHH = 800; 
const int HEIGHTT = 450;

PUBLIC void collisionDetect(Game theGame)
{
    float mW = theGame->player[theGame->playerID].width, mH = theGame->player[theGame->playerID].height;
    float mX = theGame->player[theGame->playerID].xPos, mY = theGame->player[theGame->playerID].yPos;
    
    //Don't move out of window!
    if(mX < 0)                //Left edge
        theGame->player[theGame->playerID].xPos += theGame->player[theGame->playerID].speed;
    if(mX+mW > WIDTHH)         //Right edge
         theGame->player[theGame->playerID].xPos -= theGame->player[theGame->playerID].speed;
    if(mY < 0)                //Top edge
        theGame->player[theGame->playerID].yPos += theGame->player[theGame->playerID].speed;
    if(mY+mH > HEIGHTT)        //Bottom edge
        theGame->player[theGame->playerID].yPos -= theGame->player[theGame->playerID].speed;

    //Collision with enemies
    // for(int i = 0; i < WALLCOUNT/4; i++){
    //     if(collide2d(game->man.x, game->man.y, game->bricks[i].x, game->bricks[i].y, MANSIZE, MANSIZE, 256/8, 800/8)){
    //         game->man.isDead = true;
    //         //Mix_HaltChannel(game->musicChannel);
    //         break;
    //     }
    // }

    // //Check for collision with any walls
    // for(int i = 0; i < WALLCOUNT; i++)
    // {
    //     float wX = game->walls[i].x, wY = game->walls[i].y, 
    //           wW = game->walls[i].w, wH = game->walls[i].h;

    //     if(mY+mH > wY && mY < wY+wH){
    //         //Rubbing against right edge
    //         if(mX < wX+wW && mX+mW > wX+wW){
    //             //Correct x
    //             //game->man.x = wX+wW;
    //             if(game->man.isMovingDiagonal == true){
    //                 game->man.x +=DIAGSPEED;
    //                 game->man.y +=DIAGSPEED;
    //             }
    //             else
    //                 game->man.x +=SPEED;
    //             mX = wX+wW;
    //             printf("Right Edge\n");
    //         }
    //         //Rubbing against left edge
    //         else if(mX+mW > wX && mX < wX){
    //             //Correct x
    //             //game->man.x = wX-mW;
    //             if(game->man.isMovingDiagonal == true){
    //                 game->man.x -=DIAGSPEED;
    //                 game->man.y -=DIAGSPEED;
    //             }
    //             else
    //                 game->man.x -=SPEED;
    //             mX = wX-mW;
    //             printf("Left Edge\n");
    //         }
    //     }

    //     if(mX+mW > wX && mX < wX+wW){
    //         //Are we bumping our head?
    //         if(mY < wY+wH && mY > wY){
    //             //correct y
    //             //game->man.y = wY+wH;
    //             if(game->man.isMovingDiagonal == true){
    //                 game->man.x +=DIAGSPEED;
    //                 game->man.y +=DIAGSPEED;
    //             }
    //             else
    //                 game->man.y +=SPEED;
    //             printf("Bumping head\n");
    //         }
    //         //Are we standing on the wall?
    //         else if(mY+mH > wY && mY < wY){
    //             //correct y
    //             //game->man.y = wY-mH;
    //             if(game->man.isMovingDiagonal == true){
    //                 game->man.x -=DIAGSPEED;
    //                 game->man.y -=DIAGSPEED;
    //             }
    //             else
    //                 game->man.y -=SPEED;
    //             printf("Standing on wall\n");
    //         }
    //     }
    // }
}