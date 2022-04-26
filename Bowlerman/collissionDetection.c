#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "player.h"
#include "wall.h"
#include "collissionDetection.h"


const int WDITHH = 800;  //Move eventually
const int HEIGHTT = 450;

PUBLIC void collisionDetect(Game theGame)
{
    float mW = theGame->player[theGame->playerID].width, mH = theGame->player[theGame->playerID].height;
    float mX = theGame->player[theGame->playerID].xPos, mY = theGame->player[theGame->playerID].yPos;
    int s = getPlayerSpeed(theGame->player[theGame->playerID]);
    
    //Don't move out of window!
    if(mX < 0)                //Left edge
        theGame->player[theGame->playerID].xPos += theGame->player[theGame->playerID].speed;
    if(mX+mW > WDITHH)         //Right edge
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

    //Check for collision with any walls
    for(int i = 0; i < WALLAMOUNT; i++)
    {
        float wX = theGame->wall[i].x, wY = theGame->wall[i].y, 
              wW = theGame->wall[i].w, wH = theGame->wall[i].h;

        if(mY+mH > wY && mY < wY+wH){
            //Rubbing against right edge
            if(mX < wX+wW && mX+mW > wX+wW){
                //Correct x
                theGame->player[theGame->playerID].xPos += s;
                mX = wX+wW;
                printf("Right Edge\n");
            }
            //Rubbing against left edge
            else if(mX+mW > wX && mX < wX){
                //Correct x
                theGame->player[theGame->playerID].xPos -= s;
                mX = wX-mW;
                printf("Left Edge\n");
            }
        }

        if(mX+mW > wX && mX < wX+wW){
            //Are we bumping our head?
            if(mY < wY+wH && mY > wY){
                //correct y
                theGame->player[theGame->playerID].yPos += s;
                printf("Bumping head\n");
            }
            //Are we standing on the wall?
            else if(mY+mH > wY && mY < wY){
                //correct y
                theGame->player[theGame->playerID].yPos -= s;
                printf("Standing on wall\n");
            }
        }
    }
}