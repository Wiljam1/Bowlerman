#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "player.h"
#include "collissionDetection.h"
#include "sorter.h"
#include "bomb.h"

PUBLIC Bowlingball initBomb(int playerID)
{
    //malloc(sizeof(struct bowling_ball));
    Bowlingball b;
    b.position.y = 0;
    b.position.x = 0;
    b.position.w = 70;
    b.position.h = 70;
    b.speed = 0;
    b.timervalue = 0;           //också en initiering för bomberna
    b.timerinit = 0;            //initierar timer för bomber
    b.explosioninit = 1;        // initierar explosionerna
    b.placedBombRestriction = 0; //gör så man inte kan lägga en bomb samtidigt som en är ute
    b.powerUpExplosion = 2;             //powerupp för större explosioner
    return b;
}

//timer för bomberna, går kanske att göras med SDL_ADDTimer
//och sedan SDL_TimerID
int initbowlingballtimer(int startTime, int timeAmount, int playerID)
{
    static int lastTime[4]={0}, currentTime[4] = {0};
    if(startTime != 0)
    {
        lastTime[playerID] = startTime;
    }
    currentTime[playerID] = SDL_GetTicks();
    if (currentTime[playerID] > lastTime[playerID] + timeAmount)
    {
        lastTime[playerID] = currentTime[playerID];
        return 1;
    }
    return 0;
}

int serverTimer()
{
    
}

//centrerar bombernas position, i för inkommande possition, j och k för tillfälliga variabler 
int correctBowlingBallPos(int i)
{
    int j=0, k=0;
    k=i/70;
    j=i%70;
    if(j<35){
        return (k*70);      //returnerar närmaste tile sen +3 för att få det helt centrerat
    }
    else{
        return ((k+1)*70);
    }   
}

void tryToPlaceBomb(Game theGame, int playerID)
{
    if (theGame->allowBombPlacement[playerID] == 1) // man måste veta vilken player här
    {
        theGame->allowBombPlacement[playerID] = 0;
        theGame->bombs[playerID] = initBomb(playerID);
        theGame->bombs[playerID].position.y = correctBowlingBallPos(getPlayerYPosition(theGame->player[playerID]) + 56) - 40;
        theGame->bombs[playerID].position.x = correctBowlingBallPos(getPlayerXPosition(theGame->player[playerID]) + 8);
        theGame->bombs[playerID].timervalue = initbowlingballtimer(SDL_GetTicks(), 3000, playerID); // också viktigt att veta vilken player
        theGame->bombs[playerID].timerinit = 1;           //detta värdet borde skickas som data till andra players
        theGame->bombs[playerID].placedBombRestriction = 1;
    }
}

void initExplosionPosition(Game theGame, int playerID)
{
    int tilesize = 66, diff=2; //Borde sparas i en struct för att komma åt värdet vid collisiondetection?
    SDL_Rect temp = {0,0,tilesize,tilesize};
    for(int i = 0; i < 1+4*theGame->bombs[playerID].powerUpExplosion; i++){
        theGame->explosionPosition[playerID][i].h = tilesize;
        theGame->explosionPosition[playerID][i].w = tilesize;
    }

    // I framtiden ska man väl kunna ha större explosionsradius än det vanliga?
    // Man kanske får initiera flera positioner från början men endast rendera/ha collision med de som ska visas
    //possitionen bomben är på
    int j = 0;
    theGame->explosionPosition[playerID][j].y = theGame->bombs[playerID].position.y + diff;
    theGame->explosionPosition[playerID][j].x = theGame->bombs[playerID].position.x + diff;
    //för resterande possitioner explosionen hamnar på
    for(int i=1;i<theGame->bombs[playerID].powerUpExplosion+1;i++)
    {
        
        j++;
        theGame->explosionPosition[playerID][j].y = theGame->bombs[playerID].position.y + tilesize*i + diff*2*i + diff; // Neråt
        theGame->explosionPosition[playerID][j].x = theGame->bombs[playerID].position.x + diff;
        
        j++;
        theGame->explosionPosition[playerID][j].y = theGame->bombs[playerID].position.y - tilesize*i - diff*2*i + diff;    // UPP
        theGame->explosionPosition[playerID][j].x = theGame->bombs[playerID].position.x + diff;
        
        j++;
        theGame->explosionPosition[playerID][j].y = theGame->bombs[playerID].position.y + diff;             //Höger
        theGame->explosionPosition[playerID][j].x = theGame->bombs[playerID].position.x + tilesize*i + diff*2*i + diff;
        
        j++;
        theGame->explosionPosition[playerID][j].y = theGame->bombs[playerID].position.y + diff;             //Vänster
        theGame->explosionPosition[playerID][j].x = theGame->bombs[playerID].position.x - tilesize*i - diff*2*i + diff;
        
    }
    //tar bort rect som träffar en vägg
    
    for (int i=1;i<5;i++)
    {
        if (testPossibilityToExplode(theGame, playerID, i) == 0)
        {
            for(int j=0;j<theGame->bombs[playerID].powerUpExplosion;j++)
            {
                theGame->explosionPosition[playerID][i+4*j].h = 0;
                theGame->explosionPosition[playerID][i+4*j].w = 0;
                theGame->explosionPosition[playerID][i+4*j].y = 2000;
                theGame->explosionPosition[playerID][i+4*j].x = 2000;
            }
        }
        if (testPossibilityToExplodeDestroyableWalls(theGame, playerID, i) == 0)
        {
            for(int j=1;j<theGame->bombs[playerID].powerUpExplosion;j++)
            {
                theGame->explosionPosition[playerID][i+4*j].h = 0;
                theGame->explosionPosition[playerID][i+4*j].w = 0;
                theGame->explosionPosition[playerID][i+4*j].y = 2000;
                theGame->explosionPosition[playerID][i+4*j].x = 2000;
            }
        }
        
    }
    
}

void process(Game theGame)
{
    for (int i = 0; i < 4; i++){
        if (theGame->bombs[i].timerinit == 1){
            theGame->bombs[i].timervalue = initbowlingballtimer(0, 3000, i);
            if (theGame->bombs[i].timervalue == 1){
                theGame->bombs[i].timerinit = 0;
                theGame->bombs[i].explosioninit = 0;
                initExplosionPosition(theGame, i);
                initbowlingballtimer(SDL_GetTicks(), 1000, i);
            }
        }
    }
    for (int i = 0; i < 4; i++){
        if (theGame->bombs[i].explosioninit == 0){
            theGame->bombs[i].explosioninit = initbowlingballtimer(0, 1000, i);
            for(int j=139;j<250;j++){
                if(theGame->wall[j].destroyedWall == 0){
                    theGame->wall[j].destroyedWall = testCollisionWithDestroyableWalls(theGame, j, i);
                }
            }
            if (theGame->bombs[i].explosioninit == 1){
                theGame->allowBombPlacement[i] = 1;
            }
        }
    }
}

//bomb rect possitioner i sprite sheet.
PUBLIC void loadBomb()
{
    bowlingballAnimation[ 0 ].x =   0;
    bowlingballAnimation[ 0 ].y =   0;
    bowlingballAnimation[ 0 ].w = 256;
    bowlingballAnimation[ 0 ].h = 256;

    bowlingballAnimation[ 1 ].x = 256;
    bowlingballAnimation[ 1 ].y =   0;
    bowlingballAnimation[ 1 ].w = 256;
    bowlingballAnimation[ 1 ].h = 256;

    bowlingballAnimation[ 2 ].x = 512;
    bowlingballAnimation[ 2 ].y =   0;
    bowlingballAnimation[ 2 ].w = 256;
    bowlingballAnimation[ 2 ].h = 256;

    bowlingballAnimation[ 3 ].x = 768;
    bowlingballAnimation[ 3 ].y =   0;
    bowlingballAnimation[ 3 ].w = 256;
    bowlingballAnimation[ 3 ].h = 256;

    bowlingballAnimation[ 4 ].x =   0;
    bowlingballAnimation[ 4 ].y = 256;
    bowlingballAnimation[ 4 ].w = 256;
    bowlingballAnimation[ 4 ].h = 256;

    bowlingballAnimation[ 5 ].x = 256;
    bowlingballAnimation[ 5 ].y = 256;
    bowlingballAnimation[ 5 ].w = 256;
    bowlingballAnimation[ 5 ].h = 256;

    bowlingballAnimation[ 6 ].x = 512;
    bowlingballAnimation[ 6 ].y = 256;
    bowlingballAnimation[ 6 ].w = 256;
    bowlingballAnimation[ 6 ].h = 256;

    bowlingballAnimation[ 7 ].x = 768;
    bowlingballAnimation[ 7 ].y = 256;
    bowlingballAnimation[ 7 ].w = 256;
    bowlingballAnimation[ 7 ].h = 256;

    bowlingballAnimation[ 8 ].x =   0;
    bowlingballAnimation[ 8 ].y = 512;
    bowlingballAnimation[ 8 ].w = 256;
    bowlingballAnimation[ 8 ].h = 256;

    bowlingballAnimation[ 9 ].x = 256;
    bowlingballAnimation[ 9 ].y = 512;
    bowlingballAnimation[ 9 ].w = 256;
    bowlingballAnimation[ 9 ].h = 256;

    bowlingballAnimation[ 10 ].x = 512;
    bowlingballAnimation[ 10 ].y = 512;
    bowlingballAnimation[ 10 ].w = 256;
    bowlingballAnimation[ 10 ].h = 256;

    bowlingballAnimation[ 11 ].x = 768;
    bowlingballAnimation[ 11 ].y = 512;
    bowlingballAnimation[ 11 ].w = 256;
    bowlingballAnimation[ 11 ].h = 256;

    bowlingballAnimation[ 12 ].x =   0;
    bowlingballAnimation[ 12 ].y = 768;
    bowlingballAnimation[ 12 ].w = 256;
    bowlingballAnimation[ 12 ].h = 256;

    bowlingballAnimation[ 13 ].x = 256;
    bowlingballAnimation[ 13 ].y = 768;
    bowlingballAnimation[ 13 ].w = 256;
    bowlingballAnimation[ 13 ].h = 256;

    bowlingballAnimation[ 14 ].x = 512;
    bowlingballAnimation[ 14 ].y = 768;
    bowlingballAnimation[ 14 ].w = 256;
    bowlingballAnimation[ 14 ].h = 256;

    bowlingballAnimation[ 15 ].x = 768;
    bowlingballAnimation[ 15 ].y = 768;
    bowlingballAnimation[ 15 ].w = 256;
    bowlingballAnimation[ 15 ].h = 256;

    bowlingballAnimation[ 16 ].x =   0;
    bowlingballAnimation[ 16 ].y =1024;
    bowlingballAnimation[ 16 ].w = 256;
    bowlingballAnimation[ 16 ].h = 256;

    bowlingballAnimation[ 17 ].x = 256;
    bowlingballAnimation[ 17 ].y =1024;
    bowlingballAnimation[ 17 ].w = 256;
    bowlingballAnimation[ 17 ].h = 256;
}