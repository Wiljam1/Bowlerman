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

#define YOFFSET (WIDTH/39.667)

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
    testCollosionWithExplosion(theGame, sounds);
    playerCollisionWithPowerup(theGame);
    explosionCollisionWithPowerup(theGame);       //för att förstöra powerups med explosioner
}

//collision with outer walls and the screen size
void collisionWithWallsAround(Game theGame)
{

    //"Easier to read"-variables
    
    float playerWidth = playerGetWidth(theGame->player[theGame->playerIDLocal]), playerHeight = playerGetHeight(theGame->player[theGame->playerIDLocal]);
    float playerXPos = playerGetXPosition(theGame->player[theGame->playerIDLocal]), playerYPos = playerGetYPosition(theGame->player[theGame->playerIDLocal]);
    int playerID = getPlayerID(theGame->player[theGame->playerIDLocal]);
    int speed = getPlayerSpeed(theGame->player[playerID]);
    
    //Don't move out of window!
    if(playerXPos < 0)                //Left edge
        playerSetXPos(&theGame->player[playerID], 0);
    if(playerXPos+playerWidth > WIDTH)         //Right edge
        playerSetXPos(&theGame->player[playerID], WIDTH - playerWidth);
    if(playerYPos < 0)                //Top edge
        playerSetYPos(&theGame->player[playerID], 0);
    if(playerYPos+playerHeight > HEIGHT)        //Bottom edge
        playerSetXPos(&theGame->player[playerID], HEIGHT - playerHeight);

    //Check for collision with any walls
    
    for(int i = 0; i < WALLAMOUNT; i++)
    {
        float wallXPos = getWallXPosition(theGame->wall[i]), wallYPos = getWallYPosition(theGame->wall[i]),
              wallWidth = getWallWidth(theGame->wall[i]), wallHeight = getWallHeight(theGame->wall[i]);

        if (i < 20)
        {
            if (playerYPos + playerHeight < wallYPos)
            {
                playerSetYPos(&theGame->player[playerID], wallYPos - playerHeight);
            }
            else if (playerYPos < wallYPos + YOFFSET)
            {
                playerSetYPos(&theGame->player[playerID], wallYPos + YOFFSET);
            }
        }     
        if (i >= 20 && i < 40) // Nedre
        {
            if (playerYPos + playerHeight > wallYPos)
            {
                playerSetYPos(&theGame->player[playerID], wallYPos - playerHeight);
            }
        }
        if (i >= 40 && i < 60) // Vänster
        {
            if (playerXPos < wallXPos + wallWidth)
            {
                playerSetXPos(&theGame->player[playerID], wallXPos + wallWidth);
            }

        }
        if (i >= 60 && i < 80) //höger
        {
            if (playerXPos + playerWidth > wallXPos)
            {
                playerSetXPos(&theGame->player[playerID], wallXPos - playerWidth);
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
        if (BombGetPlacedBombRestriction(theGame->bombs[i]) == 1)
        {
            playerStandingOnBomb(theGame);
        }
        if (BombGetIsPlaced(theGame->bombs[i]) == 1 && playerGetIsInvulnerable(theGame->player[i]) == false)
        {
            int id = getLocalID(theGame);
            float playerW = playerGetWidth(theGame->player[theGame->playerIDLocal]), playerH = playerGetHeight(theGame->player[theGame->playerIDLocal]);
            float playerX = playerGetXPosition(theGame->player[theGame->playerIDLocal]), playerY = playerGetYPosition(theGame->player[theGame->playerIDLocal]);
            int moveDirection = getMoveDirection(theGame->player[id]);
            int bombX = getBombXPosition(theGame->bombs[i]), bombY = getBombYPosition(theGame->bombs[i]), bombW = getBombWidth(theGame->bombs[i]), bombH = getBombHeight(theGame->bombs[i]);
            if(BombGetPlacedBombRestriction(theGame->bombs[i]) == 0)
            {
                if(moveDirection == 'w' || moveDirection == 's') //för upp och ner
                {   
                    if(playerX + playerW > bombX && playerX < bombX + bombW)
                    {
                        if(playerY + YOFFSET < bombY + bombH && playerY > bombY){
                            //correct y
                            playerSetYPos(&theGame->player[id], bombY + bombH - YOFFSET);
                        }
                        if(playerY + playerH > bombY && playerY < bombY){
                            //correct y
                            playerSetYPos(&theGame->player[id], bombY - playerH);
                        }
                    }
                }
                if(moveDirection == 'a' || moveDirection == 'd')        //för vänster och höger
                {
                    if(playerY + playerH > bombY && playerY + YOFFSET < bombY + bombH)
                    {
                        if(playerX < bombX + bombW && playerX > bombX){
                            //Correct x
                            playerSetXPos(&theGame->player[id], bombX + bombW);
                        }
                        if(playerX + playerW > bombX && playerX < bombX){
                            //Correct x
                            playerSetXPos(&theGame->player[id], bombX - playerW);
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
        int flag = 1; //vad är det för flagga egentligen? // Så att man inte kollar en spelare fyra gånger om man dör.
        if(playerGetIsInvulnerable(theGame->player[i]) == false) {

            float playerW = playerGetWidth(theGame->player[i]), playerH = playerGetHeight(theGame->player[i]);
            float playerX = playerGetXPosition(theGame->player[i]), playerY = playerGetYPosition(theGame->player[i]);
            for (int j=0;j<MAXBOMBAMOUNT;j++)
            {
                if(BombGetExplosionInit(theGame->bombs[j]) == 0) // J kan användas vid Score sen
                {
                    for (int l = 0; l < PLAYERAMOUNT; l++)
                    {
                        for (int k=0;k<(1+4*playerGetExplosionPower(theGame->player[l]));k++)
                        {
                            if(theGame->explosionPosition[j][k].x < playerX + playerW &&
                            theGame->explosionPosition[j][k].x + theGame->explosionPosition[j][k].w > playerX &&
                            theGame->explosionPosition[j][k].y < playerY + playerH &&
                            theGame->explosionPosition[j][k].h + theGame->explosionPosition[j][k].y - YOFFSET > playerY)
                            {
                                //player dead
                                if (flag == 1)      // testar med odödlighet
                                {
                                    if(getPlayerID(theGame->player[theGame->bombs[j].whoPlacedID]) != i) {
                                        playerAddScore(&theGame->player[theGame->bombs[j].whoPlacedID], 10);
                                    }
                                    setPlayerDeathFlags(theGame, i);
                                    playerDeathTimer(theGame);
                                    playDeath(s);
                                    flag = 0;
                                    
                                    checkGameOver(theGame);
                                    break;
                                }
                            }
                        }
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
        float playerW = playerGetWidth(theGame->player[playerID]), playerH = playerGetHeight(theGame->player[playerID]);
        float playerX = playerGetXPosition(theGame->player[playerID]), playerY = playerGetYPosition(theGame->player[playerID]);
        for (int i = 0;i<playerGetAmountOfBombsPlaced(theGame->player[playerID]);i++)
        {
            int bombW = getBombWidth(theGame->bombs[playerID+i*4]), bombH = getBombHeight(theGame->bombs[playerID+i*4]);
            int bombX = getBombXPosition(theGame->bombs[playerID +i*4]), bombY = getBombYPosition(theGame->bombs[playerID+i*4]);

            if(bombX < playerX + playerW &&
                bombX + bombW > playerX &&
                bombY < playerY + playerH &&
                bombH + bombY > playerY)
            {         
                BombSetPlacedBombRestriction(&theGame->bombs[playerID+i*4], 1);
            }
            else 
            {
                BombSetPlacedBombRestriction(&theGame->bombs[playerID+i*4], 0);
            }
        }
    }
}

//kollar så att en spelare inte går in i väggar i mitten av spelplanen
void testCollisionWithWalls(Game theGame)
{
    for (int i=0;i<4;i++)
    {
        float playerW = playerGetWidth(theGame->player[i]), playerH = playerGetHeight(theGame->player[i]);
        float playerX = playerGetXPosition(theGame->player[i]), playerY = playerGetYPosition(theGame->player[i]);
        int moveDirection = getMoveDirection(theGame->player[i]); 
        for (int j=100;j<250;j++)
        {

            if(WallGetDestroyedWall(theGame->wall[j]) == 0)
            {
                int wallX = getWallXPosition(theGame->wall[j]), wallY = getWallYPosition(theGame->wall[j]), wallW = getWallWidth(theGame->wall[j]), wallH = getWallHeight(theGame->wall[j]);
                if(moveDirection == 'w' || moveDirection == 's')        //för upp och ner
                {   
                    if(playerX + playerW > wallX && playerX < wallX + wallW)
                    {
                        if(playerY + YOFFSET < wallY + wallH && playerY > wallY){
                            //correct y
                            theGame->player[i].yPos = wallY + wallH - YOFFSET;
                            playerSetYPos(&theGame->player[i], wallY + wallH - YOFFSET);
                        }
                        if(playerY + playerH > wallY && playerY < wallY){
                            //correct y
                            theGame->player[i].yPos = wallY - playerH;
                            playerSetYPos(&theGame->player[i], wallY - playerH);
                        }
                    }
                }
                if(moveDirection == 'a' || moveDirection == 'd') // för vänster och höger
                {
                    if(playerY + playerH > wallY && playerY + YOFFSET < wallY + wallH)
                    {
                        if(playerX < wallX + wallW && playerX > wallX){
                            //Correct x
                            theGame->player[i].xPos = wallX + wallW;
                            playerSetXPos(&theGame->player[i], wallX + wallW);
                        }
                        if(playerX + playerW > wallX && playerX < wallX){
                            //Correct x
                            theGame->player[i].xPos = wallX - playerW;
                            playerSetXPos(&theGame->player[i], wallX - playerW);
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
    for (int i=0;i<1+4*playerGetExplosionPower(theGame->player[returnarray[j]]);i++)
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
        int bombW = getBombWidth(theGame->bombs[i]), bombH = getBombHeight(theGame->bombs[i]);
        int bombX = getBombXPosition(theGame->bombs[i]), bombY = getBombYPosition(theGame->bombs[i]);

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
        float playerW = playerGetWidth(theGame->player[playerID]), playerH = playerGetHeight(theGame->player[playerID]);
        float playerX = playerGetXPosition(theGame->player[playerID]), playerY = playerGetYPosition(theGame->player[playerID]);
        
        for (int i = 0;i<POWERUPAMOUNT;i++)
        {
            int powerUpX = PowerUpGetXPosition(theGame->powerups[i]), powerUpY = PowerUpGetYPosition(theGame->powerups[i]);
            int powerUpW = PowerUpGetWidth(theGame->powerups[i]), powerUpH = PowerUpGetHeight(theGame->powerups[i]);
            if(PowerUpGetIsPickedUp(theGame->powerups[i]) == false)   //ha något här som gör att man inte kollar exkeverade powerups
            {
                if(powerUpX < playerX + playerW &&
                    powerUpX + powerUpW > playerX &&
                    powerUpY < playerY + playerH &&
                    powerUpH + powerUpY - YOFFSET > playerY)
                {         
                    powerupGive(&theGame->player[playerID] , &theGame->powerups[i]);
                    theGame->updateFlag = true;
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
        int powerUpX = PowerUpGetXPosition(theGame->powerups[i]), powerUpY = PowerUpGetYPosition(theGame->powerups[i]);
        int powerUpW = PowerUpGetWidth(theGame->powerups[i]), powerUpH = PowerUpGetHeight(theGame->powerups[i]);
        for (int k = 0;k<POWERUPAMOUNT;k++)
        {
            for(int j=0;j<MAXBOMBAMOUNT;j++)   
            {
                if(PowerUpGetIsPickedUp(theGame->powerups[i]) == false)
                {
                    if(theGame->explosionPosition[j][k].x < powerUpX &&
                        theGame->explosionPosition[j][k].x + theGame->explosionPosition[j][k].w > powerUpX + powerUpW &&
                        theGame->explosionPosition[j][k].y < powerUpY &&
                        theGame->explosionPosition[j][k].h + theGame->explosionPosition[j][k].y > powerUpY + powerUpH)
                    {    
                        //theGame->powerups[i].indestructable = timerForPowerups(0, 1500, i);     
                        if(timerForPowerups(0, 1500, i) == false && BombGetExplosionInit(theGame->bombs[j]) == 0)
                        {
                            PowerUpSetIsPickedUp(&theGame->powerups[i], true);
                        }
                        
                    }
                }
            }
        }
    }
}

