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
    //"Easier to read"-variables
    float playerWidth = theGame->player[theGame->playerIDLocal].width, playerHeight = theGame->player[theGame->playerIDLocal].height;
    float playerXPos = theGame->player[theGame->playerIDLocal].xPos, playerYPos = theGame->player[theGame->playerIDLocal].yPos;
    int playerID = getPlayerID(theGame->player[theGame->playerIDLocal]);
    int speed = getPlayerSpeed(theGame->player[playerID]);
    
    //Don't move out of window!
    if(playerXPos < 0)                //Left edge
        theGame->player[playerID].xPos = 0;
    if(playerXPos+playerWidth > WIDTH)         //Right edge
         theGame->player[playerID].xPos = WIDTH - playerWidth;
    if(playerYPos < 0)                //Top edge
        theGame->player[playerID].yPos = 0;
    if(playerYPos+playerHeight > HEIGHT)        //Bottom edge
        theGame->player[playerID].yPos = HEIGHT - playerHeight;

    //Check for collision with any walls
    
    for(int i = 0; i < WALLAMOUNT; i++)
    {
        float wallXPos = theGame->wall[i].x, wallYPos = theGame->wall[i].y, 
              wallWidth = theGame->wall[i].w, wallHeight = theGame->wall[i].h;

        if (i < 20)
        {
            if (playerYPos + playerHeight < wallYPos)
            {
                theGame->player[playerID].yPos = wallYPos - playerHeight;
            }
            else if (playerYPos < wallYPos + 40)
            {
                theGame->player[playerID].yPos = wallYPos + 40;
            }
        }     
        if (i >= 20 && i < 40) // Nedre
        {
            if (playerYPos + playerHeight > wallYPos)
            {
                theGame->player[playerID].yPos = wallYPos - playerHeight;
            }
        }
        if (i >= 40 && i < 60) // Vänster
        {
            if (playerXPos < wallXPos + wallWidth)
            {
                theGame->player[playerID].xPos = wallXPos + wallWidth;
            }

        }
        if (i >= 60 && i < 80) //höger
        {
            if (playerXPos + playerWidth > wallXPos)
            {
                theGame->player[playerID].xPos = wallXPos - playerWidth;
            }
        }

        /*
         if(playerYPos+playerHeight > wallYPos && playerYPos < wallYPos+wallHeight)
        {
            //Rubbing against right edge
            if(playerXPos < wallXPos+wallWidth && playerXPos+playerWidth > wallXPos+wallWidth){
                //Correct xw
                theGame->player[theGame->playerIDLocal].yPos += speed;
                playerXPos = wallXPos+wallWidth;
                printf("\nRight Edge\n");
                printf("pXpos: %d\nwallXpos: %d\n", (int)playerXPos, (int)wallXPos);
            }
            //Rubbing against left edge
            else if(playerXPos+playerWidth > wallXPos && playerXPos < wallXPos){
                //Correct x
                theGame->player[theGame->playerIDLocal].xPos -= speed;
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
                theGame->player[theGame->playerIDLocal].yPos += speed;
                printf("Bumping head\n");
            }
            //Are we standing on the wall?
            else if(playerYPos+playerHeight > wallYPos && playerYPos < wallYPos){
                //correct y
                theGame->player[theGame->playerIDLocal].yPos -= speed;
                printf("standing on wall\n");
            }
        } 
        */
    }
}

// collison detection mellan bomber och spelare
// i är för antal spelare, j för antal bomber
void testCollosionWithBombs(Game theGame)
{
    if (theGame->bombs[theGame->playerIDLocal].placedBombRestriction == 1)
    {
        playerStandingOnBomb(theGame);
    }
    for (int i=0;i<4;i++)
    {
        int playerX = theGame->player[i].xPos, playerY = theGame->player[i].yPos, playerW = theGame->player[i].width, playerH = theGame->player[i].height;
        int moveDirection = theGame->player[i].moveDirection;
        for (int j=0;j<4;j++)
        {
            int bombX = theGame->bombs[j].position.x, bombY = theGame->bombs[j].position.y, bombW = theGame->bombs[j].position.w, bombH = theGame->bombs[j].position.h;
            if(theGame->bombs[j].timervalue == 0 && theGame->bombs[j].placedBombRestriction == 0)
            {
                if(moveDirection == 'w' || moveDirection == 's') 
                {   
                    if(playerX + playerW > bombX && playerX < bombX + bombW)
                    {
                        if(playerY + 30 < bombY + bombH && playerY > bombY){
                            //correct y
                            theGame->player[i].yPos = bombY + bombH - 30;
                            printf("Bumping head on bomb\n");
                        }
                        if(playerY + playerH > bombY && playerY < bombY){
                            //correct y
                            theGame->player[i].yPos = bombY - playerH;
                            printf("Standing on bomb\n");
                        }
                    }
                }
                if(moveDirection == 'a' || moveDirection == 'd') 
                {
                    if(playerY + playerH > bombY && playerY + 30 < bombY + bombH)
                    {
                        if(playerX < bombX + bombW && playerX > bombX){
                            //Correct x
                            theGame->player[i].xPos = bombX + bombW;
                            printf("Right Edge of bomb\n");
                        }
                        if(playerX + playerW > bombX && playerX < bombX){
                            //Correct x
                            theGame->player[i].xPos = bombX - playerW;
                            printf("Left Edge of bomb\n");
                        }
                    }
                }
            }
        }
    }
}

// collison detection mellan spelare och explosioner
// i är för antal spelare, j för antal bomber och k för de olika rectanglar som explosionerna finns på
void testCollosionWithExplosion(Game theGame)
{
    for (int i=0;i<4;i++)
    {
        for (int j=0;j<4;j++)
        {
            if(theGame->bombs[j].explosioninit == 0)
            {
                for (int k=0;k<1+4*theGame->bombs[i].powerUpExplosion;k++)
                {
                    if(theGame->explosionPosition[j][k].x < theGame->player[i].xPos + theGame->player[i].width &&
                       theGame->explosionPosition[j][k].x + theGame->explosionPosition[j][k].w > theGame->player[i].xPos &&
                       theGame->explosionPosition[j][k].y < theGame->player[i].yPos + theGame->player[i].height &&
                       theGame->explosionPosition[j][k].h + theGame->explosionPosition[j][k].y - 30 > theGame->player[i].yPos)
                    {
                        //player dead
                        theGame->player[i].isDead = true;
                    }
                }
            }
        }
    }
}

// om spelare släpper bomb så är kollision avstängt mellan spelaren och bomben tills man kliver av bomben
void playerStandingOnBomb(Game theGame)
{
    int playerID = getPlayerID(theGame->player[theGame->playerIDLocal]);
    for(int i=0;i<theGame->playerAmount;i++)
    {
        if(theGame->bombs[playerID].position.x < theGame->player[playerID].xPos + theGame->player[playerID].width &&
            theGame->bombs[playerID].position.x + theGame->bombs[playerID].position.w > theGame->player[playerID].xPos &&
            theGame->bombs[playerID].position.y < theGame->player[playerID].yPos + theGame->player[playerID].height &&
            theGame->bombs[playerID].position.h + theGame->bombs[playerID].position.y > theGame->player[playerID].yPos)
        {         
            theGame->bombs[playerID].placedBombRestriction = 1;
        }
        else 
        {
            theGame->bombs[playerID].placedBombRestriction = 0;
        }
    }
}

//kollar så att en spelare inte går in i väggar i mitten av spelplanen
void testCollisionWithWalls(Game theGame)
{
    for (int i=0;i<4;i++)
    {
        int playerX = theGame->player[i].xPos, playerY = theGame->player[i].yPos, playerW = theGame->player[i].width, playerH = theGame->player[i].height;
        int moveDirection = theGame->player[i].moveDirection;
        for (int j=100;j<250;j++)
        {
            if(theGame->wall[j].destroyedWall == 0)
            {
                int wallX = theGame->wall[j].x, wallY = theGame->wall[j].y, wallW = theGame->wall[j].w, wallH = theGame->wall[j].h;
                if(moveDirection == 'w' || moveDirection == 's') 
                {   
                    if(playerX + playerW > wallX && playerX < wallX + wallW)
                    {
                        if(playerY + 30 < wallY + wallH && playerY > wallY){
                            //correct y
                            theGame->player[i].yPos = wallY + wallH - 30;
                            printf("Bumping head\n");
                        }
                        if(playerY + playerH > wallY && playerY < wallY){
                            //correct y
                            theGame->player[i].yPos = wallY - playerH;
                            printf("Standing on wall\n");
                        }
                    }
                }
                if(moveDirection == 'a' || moveDirection == 'd') 
                {
                    if(playerY + playerH > wallY && playerY + 30 < wallY + wallH)
                    {
                        if(playerX < wallX + wallW && playerX > wallX){
                            //Correct x
                            theGame->player[i].xPos = wallX + wallW;
                            printf("Right Edge\n");
                        }
                        if(playerX + playerW > wallX && playerX < wallX){
                            //Correct x
                            theGame->player[i].xPos = wallX - playerW;
                            printf("Left Edge\n");
                        }
                    }
                } 
            }
        }
    }
}

//gör så att explosioner inte går in i väggar
int testCollisionExplosionWithWalls(Game theGame, int k)
{
    for(int j=0;j<4;j++)
    {
        for (int i=0;i<136;i++)
        {
            if(theGame->wall[i].x < theGame->explosionPosition[j][k].x &&
               theGame->wall[i].x + theGame->wall[i].w > theGame->explosionPosition[j][k].x + theGame->explosionPosition[j][k].w &&
               theGame->wall[i].y < theGame->explosionPosition[j][k].y &&
               theGame->wall[i].h + theGame->wall[i].y > theGame->explosionPosition[j][k].y + theGame->explosionPosition[j][k].h)
            {
                return 1;
            } 
        }
    }
    return 0;
}

//testar om väggen explosionen träffar kan förstöras och förstör den
int testCollisionWithDestroyableWalls(Game theGame, int k, int j)
{
    for (int i=0;i<1+4*theGame->bombs[j].powerUpExplosion;i++)
    {
        if(theGame->wall[k].x < theGame->explosionPosition[j][i].x &&
           theGame->wall[k].x + theGame->wall[k].w > theGame->explosionPosition[j][i].x + theGame->explosionPosition[j][i].w &&
            theGame->wall[k].y < theGame->explosionPosition[j][i].y &&
           theGame->wall[k].h + theGame->wall[k].y > theGame->explosionPosition[j][i].y + theGame->explosionPosition[j][i].h)
        {
            return 1;
        } 
    }
    return 0;
}

int testPossibilityToExplode(Game theGame, int playerID, int i)
{
    for(int k=0;k<136;k++)
    {
        if(theGame->wall[k].x < theGame->explosionPosition[playerID][i].x &&
            theGame->wall[k].x + theGame->wall[k].w > theGame->explosionPosition[playerID][i].x + theGame->explosionPosition[playerID][i].w &&
            theGame->wall[k].y < theGame->explosionPosition[playerID][i].y &&
            theGame->wall[k].h + theGame->wall[k].y > theGame->explosionPosition[playerID][i].y + theGame->explosionPosition[playerID][i].h)
        {
            return 0;
        } 
    }
    return 1;
}

int testPossibilityToExplodeDestroyableWalls(Game theGame, int playerID, int i)
{
    for(int k=139;k<250;k++)
    {
        if(theGame->wall[k].destroyedWall==0)
            {
            if(theGame->wall[k].x < theGame->explosionPosition[playerID][i].x &&
                theGame->wall[k].x + theGame->wall[k].w > theGame->explosionPosition[playerID][i].x + theGame->explosionPosition[playerID][i].w &&
                theGame->wall[k].y < theGame->explosionPosition[playerID][i].y &&
                theGame->wall[k].h + theGame->wall[k].y > theGame->explosionPosition[playerID][i].y + theGame->explosionPosition[playerID][i].h)
            {
                return 0;
            } 
        }
    }
    return 1;
}

/*
if(theGame->explosionPosition[j][k].x < theGame->wall[i].x + theGame->wall[i].w &&
            theGame->explosionPosition[j][k].x + theGame->explosionPosition[j][k].w > theGame->wall[i].x &&
            theGame->explosionPosition[j][k].y < theGame->wall[i].y + theGame->wall[i].h &&
            theGame->explosionPosition[j][k].h + theGame->explosionPosition[j][k].y > theGame->wall[i].y)
*/

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


