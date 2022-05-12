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
#include "powerup.h"
#include "bomb.h"
#include "sounds.h"

void collisionWithWallsAround(Game theGame);
void testCollosionWithBombs(Game theGame);
void testCollisionWithWalls(Game theGame);
void testCollosionWithExplosion(Game theGame, Sounds *s);
void playerCollisionWithPowerup(Game theGame);
void explosionCollisionWithPowerup(Game theGame);

PUBLIC void collisionDetect(Game theGame, Sounds *sounds)
{
    collisionWithWallsAround(theGame);
    testCollosionWithBombs(theGame);     
    testCollisionWithWalls(theGame);
    testCollosionWithExplosion(theGame, &sounds);
    playerCollisionWithPowerup(theGame);
    //explosionCollisionWithPowerup(theGame);       //för att förstöra powerups med explosioner
}

//collision with outer walls and the screen size
void collisionWithWallsAround(Game theGame)
{

    //"Easier to read"-variables
    
    float playerWidth = getPlayerWidth(theGame->player[theGame->playerIDLocal]), playerHeight = getPlayerHeight(theGame->player[theGame->playerIDLocal]);
    float playerXPos = getPlayerXPosition(theGame->player[theGame->playerIDLocal]), playerYPos = getPlayerYPosition(theGame->player[theGame->playerIDLocal]);
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
        float wallXPos = getWallXPosition(theGame->wall[i]), wallYPos = getWallYPosition(theGame->wall[i]),
              wallWidth = getWallWidth(theGame->wall[i]), wallHeight = getWallHeight(theGame->wall[i]);

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
    }
}

// collison detection mellan bomber och spelare
// i är för antal spelare, j för antal bomber
void testCollosionWithBombs(Game theGame)
{
    for (int i=0;i<MAXBOMBAMOUNT;i++)
    {
        if (theGame->bombs[i].placedBombRestriction == 1)
        {
            playerStandingOnBomb(theGame);
        }
        if (theGame->bombs[i].isPlaced == 1)
        {
            int id = getLocalID(theGame);
            float playerW = getPlayerWidth(theGame->player[theGame->playerIDLocal]), playerH = getPlayerHeight(theGame->player[theGame->playerIDLocal]);
            float playerX = getPlayerXPosition(theGame->player[theGame->playerIDLocal]), playerY = getPlayerYPosition(theGame->player[theGame->playerIDLocal]);
            int moveDirection = theGame->player[id].moveDirection;
            int bombX = getBombXPosition(theGame->bombs[i]), bombY = getBombYPosition(theGame->bombs[i]), bombW = getBombWidth(theGame->bombs[i]), bombH = getBombHeight(theGame->bombs[i]);
            if(theGame->bombs[i].placedBombRestriction == 0)
            {
                if(moveDirection == 'w' || moveDirection == 's') //för upp och ner
                {   
                    if(playerX + playerW > bombX && playerX < bombX + bombW)
                    {
                        if(playerY + 30 < bombY + bombH && playerY > bombY){
                            //correct y
                            theGame->player[id].yPos = bombY + bombH - 30;
                            printf("Bumping head on bomb\n");
                        }
                        if(playerY + playerH > bombY && playerY < bombY){
                            //correct y
                            theGame->player[id].yPos = bombY - playerH;
                            printf("Standing on bomb\n");
                        }
                    }
                }
                if(moveDirection == 'a' || moveDirection == 'd')        //för vänster och höger
                {
                    if(playerY + playerH > bombY && playerY + 30 < bombY + bombH)
                    {
                        if(playerX < bombX + bombW && playerX > bombX){
                            //Correct x
                            theGame->player[id].xPos = bombX + bombW;
                            printf("Right Edge of bomb\n");
                        }
                        if(playerX + playerW > bombX && playerX < bombX){
                            //Correct x
                            theGame->player[id].xPos = bombX - playerW;
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
void testCollosionWithExplosion(Game theGame, Sounds *s)
{
    
    for (int i=0;i<PLAYERAMOUNT;i++)
    {
    float playerW = getPlayerWidth(theGame->player[i]), playerH = getPlayerHeight(theGame->player[i]);
    float playerX = getPlayerXPosition(theGame->player[i]), playerY = getPlayerYPosition(theGame->player[i]);
        for (int j=0;j<MAXBOMBAMOUNT;j++)
        {
            if(theGame->bombs[j].explosioninit == 0)
            {
                for (int k=0;k<1+4*theGame->player[i].explosionPower;k++)
                {
                    if(theGame->explosionPosition[j][k].x < playerX + playerW &&
                       theGame->explosionPosition[j][k].x + theGame->explosionPosition[j][k].w > playerX &&
                       theGame->explosionPosition[j][k].y < playerY + playerH &&
                       theGame->explosionPosition[j][k].h + theGame->explosionPosition[j][k].y - 30 > playerY)
                    {
                        //player dead
                        theGame->player[i].isDead = true;
                        playDeath(s);
                    }
                }
            }
        }
    }
}

// om spelare släpper bomb så är kollision avstängt mellan spelaren och bomben tills man kliver av bomben
void playerStandingOnBomb(Game theGame)
{
    for(int playerID=0;playerID<theGame->playerAmount;playerID++) 
    {
        float playerW = getPlayerWidth(theGame->player[playerID]), playerH = getPlayerHeight(theGame->player[playerID]);
        float playerX = getPlayerXPosition(theGame->player[playerID]), playerY = getPlayerYPosition(theGame->player[playerID]);
        for (int i = 0;i<theGame->player[playerID].amountOfBombsPlaced;i++)
        {
            int bombW = getPlayerWidth(theGame->player[playerID+i*4]), bombH = getPlayerHeight(theGame->player[playerID+i*4]);
            int bombX = getPlayerXPosition(theGame->player[playerID+i*4]), bombY = getPlayerYPosition(theGame->player[playerID+i*4]);

            if(bombX < playerX + playerW &&
                bombX + bombW > playerX &&
                bombY < playerY + playerH &&
                bombH + bombY > playerY)
            {         
                theGame->bombs[playerID+i*4].placedBombRestriction = 1;
            }
            else 
            {
                theGame->bombs[playerID+i*4].placedBombRestriction = 0;
            }
        }
    }
}

//kollar så att en spelare inte går in i väggar i mitten av spelplanen
void testCollisionWithWalls(Game theGame)
{
    for (int i=0;i<4;i++)
    {
        float playerW = getPlayerWidth(theGame->player[i]), playerH = getPlayerHeight(theGame->player[i]);
        float playerX = getPlayerXPosition(theGame->player[i]), playerY = getPlayerYPosition(theGame->player[i]);
        int moveDirection = theGame->player[i].moveDirection; 
        for (int j=100;j<250;j++)
        {

            if(theGame->wall[j].destroyedWall == 0)
            {
                int wallX = getWallXPosition(theGame->wall[j]), wallY = getWallYPosition(theGame->wall[j]), wallW = getWallWidth(theGame->wall[j]), wallH = getWallHeight(theGame->wall[j]);
                if(moveDirection == 'w' || moveDirection == 's')        //för upp och ner
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
                if(moveDirection == 'a' || moveDirection == 'd') // för vänster och höger
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
    for(int j=0;j<MAXBOMBAMOUNT;j++)
    {
        for (int i=0;i<136;i++)
        {
            float wallXPos = getWallXPosition(theGame->wall[i]), wallYPos = getWallYPosition(theGame->wall[i]),
                wallWidth = getWallWidth(theGame->wall[i]), wallHeight = getWallHeight(theGame->wall[i]);

            if(wallXPos < theGame->explosionPosition[j][k].x &&
               wallXPos + wallWidth > theGame->explosionPosition[j][k].x + theGame->explosionPosition[j][k].w &&
               wallYPos < theGame->explosionPosition[j][k].y &&
               wallHeight + wallYPos > theGame->explosionPosition[j][k].y + theGame->explosionPosition[j][k].h)
            {
                return 1;
            } 
        }
    }
    return 0;
}

//testar om väggen explosionen träffar kan förstöras och förstör den isåfall
int testCollisionWithDestroyableWalls(Game theGame, int k, int j)
{
    float wallXPos = getWallXPosition(theGame->wall[k]), wallYPos = getWallYPosition(theGame->wall[k]),
        wallWidth = getWallWidth(theGame->wall[k]), wallHeight = getWallHeight(theGame->wall[k]);
    int returnarray[20]={0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3};
    for (int i=0;i<1+4*theGame->player[returnarray[j]].explosionPower;i++)
    {
        if(wallXPos < theGame->explosionPosition[j][i].x &&
           wallXPos + wallWidth > theGame->explosionPosition[j][i].x + theGame->explosionPosition[j][i].w &&
           wallYPos < theGame->explosionPosition[j][i].y &&
           wallHeight + wallYPos > theGame->explosionPosition[j][i].y + theGame->explosionPosition[j][i].h)
        {
            return 1;  //Wall is destroyed!
        } 
    }
    return 0;
}

//testar om det går att explodera åt sidorna samt upp o ner med oförstörbara väggar
int testPossibilityToExplode(Game theGame, int playerID, int i)
{
    for(int k=0;k<136;k++)
    {
        float wallXPos = getWallXPosition(theGame->wall[k]), wallYPos = getWallYPosition(theGame->wall[k]),
            wallWidth = getWallWidth(theGame->wall[k]), wallHeight = getWallHeight(theGame->wall[k]);

        if(wallXPos < theGame->explosionPosition[playerID][i].x &&
            wallXPos + wallWidth > theGame->explosionPosition[playerID][i].x + theGame->explosionPosition[playerID][i].w &&
            wallYPos < theGame->explosionPosition[playerID][i].y &&
            wallHeight + wallYPos > theGame->explosionPosition[playerID][i].y + theGame->explosionPosition[playerID][i].h)
        {
            return 0;
        } 
    }
    return 1;
}

//testar om det går att explodera åt sidorna samt upp o ner med förstörbara väggar
int testPossibilityToExplodeDestroyableWalls(Game theGame, int playerID, int i)
{
    for(int k=139;k<250;k++)
    {
        float wallXPos = getWallXPosition(theGame->wall[k]), wallYPos = getWallYPosition(theGame->wall[k]),
            wallWidth = getWallWidth(theGame->wall[k]), wallHeight = getWallHeight(theGame->wall[k]);
        
        if(theGame->wall[k].destroyedWall==0)
            {
            if(wallXPos < theGame->explosionPosition[playerID][i].x &&
                wallXPos + wallWidth > theGame->explosionPosition[playerID][i].x + theGame->explosionPosition[playerID][i].w &&
                wallYPos < theGame->explosionPosition[playerID][i].y &&
                wallHeight + wallYPos > theGame->explosionPosition[playerID][i].y + theGame->explosionPosition[playerID][i].h)
            {
                return 0;
            } 
        }
    }
    return 1;
}


//testar explosion med bomber
void testPossibilityToExplodeWithBombs(Game theGame, int j)
{
    
    for(int i = 0;i<MAXBOMBAMOUNT;i++)
    {
        int bombW = getPlayerWidth(theGame->player[i]), bombH = getPlayerHeight(theGame->player[i]);
        int bombX = getPlayerXPosition(theGame->player[i]), bombY = getPlayerYPosition(theGame->player[i]);

        for (int k=0;k<POWERUPAMOUNT;k++)
        {
            //if(theGame->bombs[i].timerinit == 1 && theGame->bombs[j].explosioninit == 0)
            {
                if (i != j)
                {
                    if(bombX < theGame->explosionPosition[j][k].x &&
                        bombX + bombW > theGame->explosionPosition[j][k].x + theGame->explosionPosition[j][k].w &&
                        bombY < theGame->explosionPosition[j][k].y &&
                        bombH + bombY > theGame->explosionPosition[j][k].y + theGame->explosionPosition[j][k].h)
                    {
                        theGame->bombs[i].timervalue = 1;
                    }
                } 
            }
        }
    }
}


//test collision with powerup
void playerCollisionWithPowerup(Game theGame)
{
    for(int playerID=0;playerID<theGame->playerAmount;playerID++) 
    {
        float playerW = getPlayerWidth(theGame->player[playerID]), playerH = getPlayerHeight(theGame->player[playerID]);
        float playerX = getPlayerXPosition(theGame->player[playerID]), playerY = getPlayerYPosition(theGame->player[playerID]);

        for (int i = 0;i<POWERUPAMOUNT;i++)
        {
            if(theGame->powerups[i].isPickedUp == false)   //ha något här som gör att man inte kollar exkeverade powerups
            {
                if(theGame->powerups[i].x < playerX + playerW &&
                    theGame->powerups[i].x + theGame->powerups[i].w > playerX &&
                    theGame->powerups[i].y < playerY + playerH &&
                    theGame->powerups[i].h + theGame->powerups[i].y - 30 > playerY)
                {         
                    powerupGive(theGame, playerID, i);
                }
            }
        }
    }
}

//collision mellan powerups och explosioner
//funkar inte ännu, måste ändra något så att explosionerna inte tar ut powerups direkt
void explosionCollisionWithPowerup(Game theGame)
{
    for(int i=0;i<POWERUPAMOUNT;i++) 
    {
        for (int k = 0;k<POWERUPAMOUNT;k++)
        {
            for(int j=0;j<MAXBOMBAMOUNT;j++)   
            {
                if(theGame->powerups[i].isPickedUp == false)
                {
                    if(theGame->explosionPosition[j][k].x < theGame->powerups[i].x &&
                        theGame->explosionPosition[j][k].x + theGame->explosionPosition[j][k].w > theGame->powerups[i].x + theGame->powerups[i].w &&
                        theGame->explosionPosition[j][k].y < theGame->powerups[i].y &&
                        theGame->explosionPosition[j][k].h + theGame->explosionPosition[j][k].y > theGame->powerups[i].y + theGame->powerups[i].h)
                    {         
                        theGame->powerups[i].isPickedUp = true;
                    }
                }
            }
        }
    }
}

