#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "player.h"
#include "wall.h"
#include "collisionDetection.h"
#include "powerup.h"
#include "bomb.h"
#include "sounds.h"

#define YOFFSET (WIDTH/39.5)

//collision detection with the walls around the map
void collisionWithWallsAround(Game theGame, Player p[]);
//collision detection between bombs and players
void testCollisionWithBombs(Game theGame, Player p[]);
//collision detection between players and walls in the middle of the map
void testCollisionWithWalls(Game theGame, Player p[]);
//collision detection between player and explosions
void testCollisionWithExplosion(Game theGame, Sounds s, Player p[]);
//collision detection with players and powerups
void playerCollisionWithPowerup(Game theGame, Player p[]);
//collision detection between explosions and powerups
void explosionCollisionWithPowerup(Game theGame);
//collision detection for players standing on recently dropped bomb
void playerStandingOnBomb(Game theGame, Player p[]);                 

PUBLIC void collisionDetect(Game theGame, Sounds sounds, Player players[])
{
    collisionWithWallsAround(theGame, players);
    testCollisionWithBombs(theGame, players);     
    testCollisionWithWalls(theGame, players);
    testCollisionWithExplosion(theGame, sounds, players);
    playerCollisionWithPowerup(theGame, players);
    explosionCollisionWithPowerup(theGame);       
}

void collisionWithWallsAround(Game theGame, Player player[])
{
    int id = theGame->playerIDLocal;
    //"Easier to read"-variables
    
    float playerWidth = playerGetWidth(player, id), playerHeight = playerGetHeight(player, id);
    float playerXPos = playerGetXPosition(player, id), playerYPos = playerGetYPosition(player, id);
    int speed = getPlayerSpeed(player, id);
    
    //Don't move out of window!
    if(playerXPos < 0)                //Left edge
        playerSetXPos(player, id, 0);
    if(playerXPos+playerWidth > WIDTH)         //Right edge
        playerSetXPos(player, id, WIDTH - playerWidth);
    if(playerYPos < 0)                //Top edge
        playerSetYPos(player, id, 0);
    if(playerYPos+playerHeight > HEIGHT)        //Bottom edge
        playerSetXPos(player, id, HEIGHT - playerHeight);

    //Check for collision with any walls
    
    for(int i = 0; i < WALLAMOUNT; i++)
    {
        float wallXPos = getWallXPosition(theGame->wall[i]), wallYPos = getWallYPosition(theGame->wall[i]),
              wallWidth = getWallWidth(theGame->wall[i]), wallHeight = getWallHeight(theGame->wall[i]);

        if (i < 20)
        {
            if (playerYPos + playerHeight < wallYPos)
            {
                playerSetYPos(player, id, wallYPos - playerHeight);
            }
            else if (playerYPos < wallYPos + YOFFSET)
            {
                playerSetYPos(player, id, wallYPos + YOFFSET);
            }
        }     
        if (i >= 20 && i < 40) // Nedre
        {
            if (playerYPos + playerHeight > wallYPos)
            {
                playerSetYPos(player, id, wallYPos - playerHeight);
            }
        }
        if (i >= 40 && i < 60) // Vänster
        {
            if (playerXPos < wallXPos + wallWidth)
            {
                playerSetXPos(player, id, wallXPos + wallWidth);
            }

        }
        if (i >= 60 && i < 80) //höger
        {
            if (playerXPos + playerWidth > wallXPos)
            {
                playerSetXPos(player, id, wallXPos - playerWidth);
            }
        }
    }
}

void testCollisionWithBombs(Game theGame, Player player[])
{
    for (int i=0;i<MAXBOMBAMOUNT;i++)
    {
        if (BombGetPlacedBombRestriction(theGame->bombs[i]) == 1)
        {
            playerStandingOnBomb(theGame, player);
        }
        if (BombGetIsPlaced(theGame->bombs[i]) == 1)
        {
            int id = gameGetLocalID(theGame);
            float playerW = playerGetWidth(player, id), playerH = playerGetHeight(player, id);
            float playerX = playerGetXPosition(player, id), playerY = playerGetYPosition(player, id);
            char moveDirection = playerGetMoveDirection(player, id);
            int bombX = getBombXPosition(theGame->bombs[i]), bombY = getBombYPosition(theGame->bombs[i]), bombW = getBombWidth(theGame->bombs[i]), bombH = getBombHeight(theGame->bombs[i]);
            if(BombGetPlacedBombRestriction(theGame->bombs[i]) == 0)
            {
                if(moveDirection == 'w' || moveDirection == 's') 
                {   
                    if(playerX + playerW > bombX && playerX < bombX + bombW)
                    {
                        if(playerY + YOFFSET < bombY + bombH && playerY > bombY){
                            //correct y
                            playerSetYPos(player, id, bombY + bombH - YOFFSET);
                        }
                        if(playerY + playerH > bombY && playerY < bombY){
                            //correct y
                            playerSetYPos(player, id, bombY - playerH);
                        }
                    }
                }
                if(moveDirection == 'a' || moveDirection == 'd')    
                {
                    if(playerY + playerH > bombY && playerY + YOFFSET < bombY + bombH)
                    {
                        if(playerX < bombX + bombW && playerX > bombX){
                            //Correct x
                            playerSetXPos(player, id, bombX + bombW);
                        }
                        if(playerX + playerW > bombX && playerX < bombX){
                            //Correct x
                            playerSetXPos(player, id, bombX - playerW);
                        }
                    }
                }
            }
        }
    }
}

void testCollisionWithExplosion(Game theGame, Sounds s, Player player[])
{
    for (int i=0;i<PLAYERAMOUNT;i++)
    {
        //Flag to make sure only one timer starts when a player dies, instead of four.
        int flag = 1;
        if(playerGetIsInvulnerable(player, i) == false) {

            float playerW = playerGetWidth(player, i), playerH = playerGetHeight(player, i);
            float playerX = playerGetXPosition(player, i), playerY = playerGetYPosition(player, i);
            for (int j=0;j<MAXBOMBAMOUNT;j++)
            {
                if(BombGetExplosionInit(theGame->bombs[j]) == 0) 
                {
                    for (int l = 0; l < PLAYERAMOUNT; l++)
                    {
                        for (int k=0;k<(1+4*playerGetExplosionPower(player, l));k++)
                        {
                            if(theGame->explosionPosition[j][k].x < playerX + playerW &&
                            theGame->explosionPosition[j][k].x + theGame->explosionPosition[j][k].w > playerX &&
                            theGame->explosionPosition[j][k].y < playerY + playerH &&
                            theGame->explosionPosition[j][k].h + theGame->explosionPosition[j][k].y - YOFFSET > playerY)
                            {
                                //player dead
                                if (flag == 1 && !playerGetIsDead(player, i))  
                                {
                                    if(getPlayerID(player, theGame->bombs[j].whoPlacedID) != i) {
                                        //Increase score by 10 for each player killed
                                        playerAddScore(player, theGame->bombs[j].whoPlacedID, 10);      
                                    }
                                    else if(getPlayerID(player, theGame->bombs[j].whoPlacedID == i)){
                                        //Give every other player 3 score if killed by own bomb
                                        playerAddScore(player, theGame->bombs[j].whoPlacedID, -3);       
                                        for(int count = 0; count < theGame->playerAmount; count++){
                                            playerAddScore(player, count, 3);
                                        }
                                    }
                                    gameUpdateFlagSet(theGame, true);
                                    setPlayerDeathFlags(theGame, player, i);
                                    playerDeathTimer(theGame, player);
                                    playDeath(s);
                                    flag = 0;
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

void playerStandingOnBomb(Game theGame, Player player[])
{
    for(int playerID=0;playerID<theGame->playerAmount;playerID++) 
    {
        float playerW = playerGetWidth(player, playerID), playerH = playerGetHeight(player, playerID);
        float playerX = playerGetXPosition(player, playerID), playerY = playerGetYPosition(player, playerID);
        for (int i = 0;i<playerGetAmountOfBombsPlaced(player, playerID);i++)
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

void testCollisionWithWalls(Game theGame, Player player[])
{

    int i = gameGetLocalID(theGame);

    float playerW = playerGetWidth(player, i), playerH = playerGetHeight(player, i);
    float playerX = playerGetXPosition(player, i), playerY = playerGetYPosition(player, i);
    char moveDirection = playerGetMoveDirection(player, i); 
    for (int j=100;j<250;j++)
    {

        if(WallGetDestroyedWall(theGame->wall[j]) == 0)
        {
            float wallX = getWallXPosition(theGame->wall[j]), wallY = getWallYPosition(theGame->wall[j]), wallW = getWallWidth(theGame->wall[j]), wallH = getWallHeight(theGame->wall[j]);
            if(moveDirection == 'w' || moveDirection == 's')  
            {   
                if(playerX + playerW > wallX && playerX < wallX + wallW)
                {
                    if(playerY + YOFFSET < wallY + wallH && playerY > wallY){
                        //correct y
                        playerSetYPos(player, i, wallY + wallH - YOFFSET);
                    }
                    if(playerY + playerH > wallY && playerY < wallY){
                        //correct y
                        playerSetYPos(player, i, wallY - playerH);
                    }
                }
            }
            if(moveDirection == 'a' || moveDirection == 'd')
            {
                if(playerY + playerH > wallY && playerY + YOFFSET < wallY + wallH)
                {
                    if(playerX < wallX + wallW && playerX > wallX){
                        //Correct x
                        playerSetXPos(player, i, wallX + wallW);
                    }
                    if(playerX + playerW > wallX && playerX < wallX){
                        //Correct x
                        playerSetXPos(player, i, wallX - playerW);
                    }
                }
            } 
        }
    }

}

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

int testCollisionWithDestroyableWalls(Game theGame, Player player[], int k, int j)
{
    float wallXPos = getWallXPosition(theGame->wall[k]), wallYPos = getWallYPosition(theGame->wall[k]),
        wallWidth = getWallWidth(theGame->wall[k]), wallHeight = getWallHeight(theGame->wall[k]);
    int returnarray[20]={0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3};
    for (int i=0;i<1+4*playerGetExplosionPower(player, returnarray[j]);i++)
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

void testPossibilityToExplodeWithBombs(Game theGame, int j)
{
    
    for(int i = 0;i<MAXBOMBAMOUNT;i++)
    {
        int bombW = getBombWidth(theGame->bombs[i]), bombH = getBombHeight(theGame->bombs[i]);
        int bombX = getBombXPosition(theGame->bombs[i]), bombY = getBombYPosition(theGame->bombs[i]);

        for (int k=0;k<POWERUPAMOUNT;k++)
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

void playerCollisionWithPowerup(Game theGame, Player player[])
{
    for(int playerID=0;playerID<theGame->playerAmount;playerID++) 
    {
        float playerW = playerGetWidth(player, playerID), playerH = playerGetHeight(player, playerID);
        float playerX = playerGetXPosition(player, playerID), playerY = playerGetYPosition(player, playerID);
        
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
                    powerupGive(player, playerID, &theGame->powerups[i]);
                    theGame->updateFlag = true;
                    
                }
            }
        }
    }
}

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
