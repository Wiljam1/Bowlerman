#include <sDL2/sDL.h>
#include <sDL2/sDL_image.h>
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
    float playerWidth = theGame->player[theGame->playerID].width, playerHeight = theGame->player[theGame->playerID].height;
    float playerXPos = theGame->player[theGame->playerID].xPos, playerYPos = theGame->player[theGame->playerID].yPos;
    int speed = getPlayerSpeed(theGame->player[theGame->playerID]);
    
    //Don't move out of window!
    if(playerXPos < 0)                //Left edge
        theGame->player[theGame->playerID].xPos = 0;
    if(playerXPos+playerWidth > WDITHH)         //Right edge
         theGame->player[theGame->playerID].xPos = WDITHH - playerWidth;
    if(playerYPos < 0)                //Top edge
        theGame->player[theGame->playerID].yPos = 0;
    if(playerYPos+playerHeight > HEIGHTT)        //Bottom edge
        theGame->player[theGame->playerID].yPos = HEIGHTT - playerHeight;

    //Collision with enemies
    // for(int i = 0; i < WALLCOUNT/4; i++){
    //     if(collide2d(game->man.x, game->man.y, game->bricks[i].x, game->bricks[i].y, MANsIZE, MANsIZE, 256/8, 800/8)){
    //         game->man.isDead = true;
    //         //Mix_HaltChannel(game->musicChannel);
    //         break;
    //     }
    // }

    //Check for collision with any walls
    for(int i = 0; i < WALLAMOUNT; i++)
    {
        float wallXPos = theGame->wall[i].x, wallYPos = theGame->wall[i].y, 
              wallWidth = theGame->wall[i].w, wallHeight = theGame->wall[i].h;
              
        if (i >= 20 && i < 40)
        {
            if (playerYPos + playerHeight > wallYPos)
            {
                theGame->player[theGame->playerID].yPos = wallYPos - playerHeight;
            }
        }
        if (i >= 40 && i < 60)
        {
            if (playerXPos < wallXPos + wallWidth)
            {
                theGame->player[theGame->playerID].xPos = wallXPos + wallWidth;
            }

        }
        if (i >= 60 && i < 80)
        {
            if (playerXPos + playerWidth > wallXPos)
            {
                theGame->player[theGame->playerID].xPos = wallXPos - playerWidth;
            }
        }





        /* if(playerYPos+playerHeight > wallYPos && playerYPos < wallYPos+wallHeight)
        {
            //Rubbing against right edge
            if(playerXPos < wallXPos+wallWidth && playerXPos+playerWidth > wallXPos+wallWidth){
                //Correct xw
                theGame->player[theGame->playerID].yPos += speed;
                playerXPos = wallXPos+wallWidth;
                printf("\nRight Edge\n");
                printf("pXpos: %d\nwallXpos: %d\n", (int)playerXPos, (int)wallXPos);
            }
            //Rubbing against left edge
            else if(playerXPos+playerWidth > wallXPos && playerXPos < wallXPos){
                //Correct x
                theGame->player[theGame->playerID].xPos -= speed;
                playerXPos = wallXPos-playerWidth;
                printf("Left Edge\n");
            }
        }



        if(playerXPos+playerWidth > wallXPos && playerXPos < wallXPos+wallWidth)
        {
            //Are we bumping our head?
            printf("HEj");
            if(playerYPos < wallYPos+wallHeight && playerYPos > wallYPos){
                //correct y
                theGame->player[theGame->playerID].yPos += speed;
                printf("Bumping head\n");
            }
            //Are we standing on the wall?
            else if(playerYPos+playerHeight > wallYPos && playerYPos < wallYPos){
                //correct y
                theGame->player[theGame->playerID].yPos -= speed;
                printf("standing on wall\n");
            }
        } */
    }
}