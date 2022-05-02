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


PUBLIC void collisionDetect(Game theGame)
{
    float playerWidth = theGame->player[theGame->playerID].width, playerHeight = theGame->player[theGame->playerID].height;
    float playerXPos = theGame->player[theGame->playerID].xPos, playerYPos = theGame->player[theGame->playerID].yPos;
    int speed = getPlayerSpeed(theGame->player[theGame->playerID]);
    
    //Don't move out of window!
    if(playerXPos < 0)                //Left edge
        theGame->player[theGame->playerID].xPos = 0;
    if(playerXPos+playerWidth > WIDTH)         //Right edge
         theGame->player[theGame->playerID].xPos = WIDTH - playerWidth;
    if(playerYPos < 0)                //Top edge
        theGame->player[theGame->playerID].yPos = 0;
    if(playerYPos+playerHeight > HEIGHT)        //Bottom edge
        theGame->player[theGame->playerID].yPos = HEIGHT - playerHeight;

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

        if (i < 20)
        {
            if (playerYPos < wallYPos)
            {
                theGame->player[theGame->playerID].yPos = wallYPos;
            }
        }     
        if (i >= 20 && i < 40) // Nedre
        {
            if (playerYPos + playerHeight > wallYPos)
            {
                theGame->player[theGame->playerID].yPos = wallYPos - playerHeight;
            }
        }
        if (i >= 40 && i < 60) // Vänster
        {
            if (playerXPos < wallXPos + wallWidth)
            {
                theGame->player[theGame->playerID].xPos = wallXPos + wallWidth;
            }

        }
        if (i >= 60 && i < 80) //höger
        {
            if (playerXPos + playerWidth > wallXPos)
            {
                theGame->player[theGame->playerID].xPos = wallXPos - playerWidth;
            }
        }




        /*
         if(playerYPos+playerHeight > wallYPos && playerYPos < wallYPos+wallHeight)
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
        } 
        */
    }
}

void testCollosionWithBombs(Game theGame)
{
    for (int i=0;i<4;i++)
    {
        for (int j=0;j<4;j++)
        {
            if(theGame->bombs[j].timervalue == 0)
            {
                if(theGame->player[i].moveDirection == 'w' || theGame->player[i].moveDirection == 's') 
                {   
                    if(theGame->player[i].xPos+theGame->player[i].width > theGame->bombs[j].position.x && theGame->player[i].xPos < theGame->bombs[j].position.x+theGame->bombs[j].position.w)
                    {
                        if(theGame->player[i].yPos < theGame->bombs[j].position.y + theGame->bombs[j].position.h && theGame->player[i].yPos > theGame->bombs[j].position.y){
                            //correct y
                            
                            theGame->player[i].yPos = theGame->bombs[j].position.y + theGame->bombs[j].position.h;
                            printf("Bumping head\n");
                        }
                        //Are we standing on the wall?
                        if(theGame->player[i].yPos + theGame->player[i].height > theGame->bombs[j].position.y && theGame->player[i].yPos < theGame->bombs[j].position.y){
                            //correct y

                            theGame->player[i].yPos = theGame->bombs[j].position.y - theGame->player[i].height;
                            printf("Standing on wall\n");
                        }
                    }
                }
                if(theGame->player[i].moveDirection == 'a' || theGame->player[i].moveDirection == 'd') 
                {
                    if(theGame->player[i].yPos + theGame->player[i].height > theGame->bombs[j].position.y && theGame->player[i].yPos < theGame->bombs[j].position.y + theGame->bombs[j].position.h)
                    {
                        //Rubbing against right edge
                        if(theGame->player[i].xPos < theGame->bombs[j].position.x + theGame->bombs[j].position.w && theGame->player[i].xPos > theGame->bombs[j].position.x){
                            //Correct xw
                            theGame->player[i].xPos = theGame->bombs[j].position.x + theGame->bombs[j].position.w;
                            printf("Right Edge\n");
                        }
                        //Rubbing against left edge
                        if(theGame->player[i].xPos + theGame->player[i].width > theGame->bombs[j].position.x && theGame->player[i].xPos < theGame->bombs[j].position.x){
                            //Correct x
                            theGame->player[i].xPos = theGame->bombs[j].position.x - theGame->player[i].width;
                            printf("Left Edge\n");
                        }
                    }
                }
            }
        }
    }
}

/*
void testCollosionWithExplosion(Game theGame)
{
    for (int i=0;i<4;i++)
    {
        for (int j=0;j<4;j++)
        {
            if(theGame->bombs[j].explosioninit == 0)
            {
                for (int k=0;k<5;k++)
                {
                    if(theGame->player[i].moveDirection == 'w' || theGame->player[i].moveDirection == 's') 
                    {   
                        if(theGame->player[i].xPos+theGame->player[i].width > theGame->explosionPosition[i][k].x && theGame->player[i].xPos < theGame->explosionPosition[i][k].x+theGame->explosionPosition[i][k].w)
                        {
                            if(theGame->player[i].yPos < theGame->explosionPosition[i][k].y + theGame->explosionPosition[i][k].h && theGame->player[i].yPos > theGame->explosionPosition[i][k].y){
                                //player dead
                                
                                theGame->player[i].yPos = 500;
                                theGame->player[i].xPos = 500;
                                return;
                            }
                            //Are we standing on the wall?
                            if(theGame->player[i].yPos + theGame->player[i].height > theGame->explosionPosition[i][k].y && theGame->player[i].yPos < theGame->explosionPosition[i][k].y){
                                //player dead

                                theGame->player[i].yPos = 500;
                                theGame->player[i].xPos = 500;
                                return;
                            }
                        }
                    }
                    if(theGame->player[i].moveDirection == 'a' || theGame->player[i].moveDirection == 'd' || theGame->player[i].moveDirection == '0') 
                    {
                        if(theGame->player[i].yPos + theGame->player[i].height > theGame->explosionPosition[i][k].y && theGame->player[i].yPos < theGame->explosionPosition[i][k].y + theGame->explosionPosition[i][k].h)
                        {
                            
                            if(theGame->player[i].xPos < theGame->explosionPosition[i][k].x + theGame->explosionPosition[i][k].w && theGame->player[i].xPos > theGame->explosionPosition[i][k].x){
                                //player dead

                                theGame->player[i].yPos = 500;
                                theGame->player[i].xPos = 500;
                                return;
                            }
                            
                            if(theGame->player[i].xPos + theGame->player[i].width > theGame->explosionPosition[i][k].x && theGame->player[i].xPos < theGame->explosionPosition[i][k].x){
                                //player dead

                                theGame->player[i].yPos = 500;
                                theGame->player[i].xPos = 500;
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}
*/
void testCollosionWithExplosion(Game theGame)
{
    for (int i=0;i<4;i++)
    {
        for (int j=0;j<4;j++)
        {
            if(theGame->bombs[j].explosioninit == 0)
            {
                for (int k=0;k<5;k++)
                {
                    if(theGame->explosionPosition[i][k].x < theGame->player[i].xPos + theGame->player[i].width &&
                       theGame->explosionPosition[i][k].x + theGame->explosionPosition[i][k].w > theGame->player[i].xPos &&
                       theGame->explosionPosition[i][k].y < theGame->player[i].yPos + theGame->player[i].height &&
                       theGame->explosionPosition[i][k].h + theGame->explosionPosition[i][k].y > theGame->player[i].yPos)
                    {
                        //player dead
                        theGame->player[i].yPos = 500;
                        theGame->player[i].xPos = 500;
                        return;
                    }
                }
            }
        }
    }
}


