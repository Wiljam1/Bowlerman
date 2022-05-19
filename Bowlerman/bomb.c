#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "player.h"
#include "collissionDetection.h"
#include "powerup.h"
#include "bomb.h"
#include "sounds.h"
#include "wall.h"
#define BOMBTIMER 3000
#define TILESIZE (WIDTH / 17)
#define DIFF (WIDTH / 595)

PUBLIC Bowlingball initBomb();

PUBLIC Bowlingball initBomb()
{
    //malloc(sizeof(struct bowling_ball));
    Bowlingball b;
    b.position.y = 0;
    b.position.x = 0;
    b.position.w = (WIDTH / 17);
    b.position.h = (WIDTH / 17);
    b.speed = 0;
    b.timervalue = 0;           //också en initiering för bomberna
    b.timerinit = 1;            //initierar timer för bomber
    b.explosioninit = 1;        // initierar explosionerna
    b.placedBombRestriction = 1; //gör så man inte kan lägga en bomb samtidigt som en är ute             
    b.isPlaced = 1;
    b.startvaluetimerbomb = 0;
    b.whoPlacedID = 0;
    return b;
}

//timer för bomberna, går kanske att göras med SDL_ADDTimer
//och sedan SDL_TimerID
int initbowlingballtimer(int startTime, int timeAmount, int playerID)
{
    static int lastTime[MAXBOMBAMOUNT] = {0}, currentTime[MAXBOMBAMOUNT] = {0};
    if(startTime != 0)
    {
        lastTime[playerID] = startTime;
    }
    currentTime[playerID] = SDL_GetTicks();
    if (currentTime[playerID] > lastTime[playerID] + timeAmount)
    {
        //lastTime[playerID] = currentTime[playerID];
        return 1;               //returnar 1 om tiden är ute
    }
    return 0;
}

//försöker lägga ut en bomb, går det så görs det även.
//tar platsen från spelaren samt initierar timer
void tryToPlaceBomb(Game theGame, int playerID)
{
    int amount = 0;
    amount = playerGetAmountOfBombsPlaced(theGame->player[playerID])*4;
    
    if (playerGetAmountOfBombsPlaced(theGame->player[playerID]) < playerGetAmountOfBombs(theGame->player[playerID]) && playerGetIsInvulnerable(theGame->player[playerID]) == false) // man måste veta vilken player här
    {
        if(BombGetIsPlaced(theGame->bombs[playerID+amount]) == 0)
        {
            int bombalreadyplaced=0;
            for(int i=0;i<MAXBOMBAMOUNT;i++)
            {
                if (getBombYPosition(theGame->bombs[i]) == correctBowlingBallPosy(playerGetYPosition(theGame->player[playerID])) &&
                    getBombXPosition(theGame->bombs[i]) == correctBowlingBallPosx(playerGetXPosition(theGame->player[playerID])) && BombGetIsPlaced(theGame->bombs[i]) == 1){
                        bombalreadyplaced = 0;
                        break;
                }
                else{
                    bombalreadyplaced = 1;
                }
            }
            if(bombalreadyplaced == 1)
            {
                theGame->bombs[playerID+amount] = initBomb();
                BombSetYPosition(&theGame->bombs[playerID+amount], correctBowlingBallPosy(playerGetYPosition(theGame->player[playerID])));
                BombSetXPosition(&theGame->bombs[playerID+amount], correctBowlingBallPosx(playerGetXPosition(theGame->player[playerID]))); 
                BombSetTimerValue(&theGame->bombs[playerID+amount], initbowlingballtimer(SDL_GetTicks(), BOMBTIMER, playerID+amount));
                BombSetWhoPlacedID(&theGame->bombs[playerID+amount], playerID);
                playerAddAmountOfBombsPlaced(&theGame->player[playerID], 1);                //antal bomber som är placerade
                BombSetStartvaluetimerbomb(&theGame->bombs[playerID+amount], SDL_GetTicks()); 
            }
            
        }
    }
}

//sorterar bomber så att sprängda bomber åker iväg, funkar inte helt med timer dock
void sortBombsArray(Game theGame,int i)
{
    Bowlingball tmp;
    int temporarytime=0;
    for (int k=0;k<playerGetAmountOfBombsPlaced(theGame->player[i]);k++)
    {
        for (int j = 1;j<playerGetAmountOfBombsPlaced(theGame->player[i])+1;j++)
        {
            if(BombGetIsPlaced(theGame->bombs[i+j*4]) == 1)
            {
                if(BombGetIsPlaced(theGame->bombs[i+j*4-4]) == 0)
                {
                    initbowlingballtimer(theGame->bombs[i+j*4].startvaluetimerbomb, BOMBTIMER, i+j*4-4);
                    tmp = theGame->bombs[i+j*4-4];
                    theGame->bombs[i+j*4-4] = theGame->bombs[i+j*4];
                    theGame->bombs[i+j*4] = tmp;
                    theGame->bombs[i+j*4-4].isPlaced == 1;
                    theGame->bombs[i+j*4].isPlaced == 0;
                    //initExplosionPosition(theGame, i+j*4);
                    //initExplosionPosition(theGame, i+j*4-4);
                    /*
                    int a=0;
                    theGame->explosionPosition[i+j*4-4][a] = theGame->explosionPosition[i+j*4][a];
                    for(int l=1;l<playerGetExplosionPower(theGame->player[i+j*4])+1;l++)
                    {
                        a++;
                        theGame->explosionPosition[i+j*4-4][a] = theGame->explosionPosition[i+j*4][a];
                        a++;
                        theGame->explosionPosition[i+j*4-4][a] = theGame->explosionPosition[i+j*4][a];
                        a++;
                        theGame->explosionPosition[i+j*4-4][a] = theGame->explosionPosition[i+j*4][a];
                        a++;
                        theGame->explosionPosition[i+j*4-4][a] = theGame->explosionPosition[i+j*4][a];
                    }
                    */
                }
            }
        }
    }
}

//sätter positionerna för explosionen efter vart bomben legat
void initExplosionPosition(Game theGame, int playerID)
{
    int tilesize = (TILESIZE - 4), diff=2; //Borde sparas i en struct för att komma åt värdet vid collisiondetection?
    int returnarray[20]={0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3};
    SDL_Rect temp = {0,0,tilesize,tilesize};
    for(int i = 0; i < 1+4*playerGetExplosionPower(theGame->player[returnarray[playerID]]); i++){
        theGame->explosionPosition[playerID][i].h = tilesize;
        theGame->explosionPosition[playerID][i].w = tilesize;
    }

    // I framtiden ska man väl kunna ha större explosionsradius än det vanliga?
    // Man kanske får initiera flera positioner från början men endast rendera/ha collision med de som ska visas
    //possitionen bomben är på
    int j = 0;
    theGame->explosionPosition[playerID][j].y = getBombYPosition(theGame->bombs[playerID]) + diff;
    theGame->explosionPosition[playerID][j].x = getBombXPosition(theGame->bombs[playerID]) + diff;
    //för resterande possitioner explosionen hamnar på
    for(int i=1;i<playerGetExplosionPower(theGame->player[returnarray[playerID]])+1;i++)
    {
        
        j++;
        theGame->explosionPosition[playerID][j].y = getBombYPosition(theGame->bombs[playerID]) + tilesize*i + diff*2*i + diff; // Neråt
        theGame->explosionPosition[playerID][j].x = getBombXPosition(theGame->bombs[playerID]) + diff;
        
        j++;
        theGame->explosionPosition[playerID][j].y = getBombYPosition(theGame->bombs[playerID]) - tilesize*i - diff*2*i + diff;    // UPP
        theGame->explosionPosition[playerID][j].x = getBombXPosition(theGame->bombs[playerID]) + diff;
        
        j++;
        theGame->explosionPosition[playerID][j].y = getBombYPosition(theGame->bombs[playerID]) + diff;             //Höger
        theGame->explosionPosition[playerID][j].x = getBombXPosition(theGame->bombs[playerID]) + tilesize*i + diff*2*i + diff;
        
        j++;
        theGame->explosionPosition[playerID][j].y = getBombYPosition(theGame->bombs[playerID]) + diff;             //Vänster
        theGame->explosionPosition[playerID][j].x = getBombXPosition(theGame->bombs[playerID]) - tilesize*i - diff*2*i + diff;
        
    }
    //tar bort rect som träffar en vägg
    
    for (int i=1;i<1+4*playerGetExplosionPower(theGame->player[returnarray[playerID]]);i++)
    {
        if (testPossibilityToExplode(theGame, playerID, i) == 0)        //för oförstörbara väggar
        {
            for(int j=0;j<playerGetExplosionPower(theGame->player[returnarray[playerID]]);j++)
            {
                theGame->explosionPosition[playerID][i+4*j].h = 0;
                theGame->explosionPosition[playerID][i+4*j].w = 0;
                theGame->explosionPosition[playerID][i+4*j].y = 2000;
                theGame->explosionPosition[playerID][i+4*j].x = 2000;
            }
        }
        if (testPossibilityToExplodeDestroyableWalls(theGame, playerID, i) == 0)    //för förstörbara väggar
        {
            for(int j=1;j<playerGetExplosionPower(theGame->player[returnarray[playerID]]);j++)
            {
                theGame->explosionPosition[playerID][i+4*j].h = 0;
                theGame->explosionPosition[playerID][i+4*j].w = 0;
                theGame->explosionPosition[playerID][i+4*j].y = 2000;
                theGame->explosionPosition[playerID][i+4*j].x = 2000;
            }
        }
        
    }
    
}

//centrerar bombernas position, i för inkommande possition, j och k för tillfälliga variabler 
int correctBowlingBallPosx(int i)
{
    int size = WIDTH / 17;
    int j=0, k=0;
    i -= WIDTH/99;
    k=i/size;
    j=i%size;
    if(j<35){
        return (k*size);      
    }
    else{
        return ((k+1)*size);
    }   
}


int correctBowlingBallPosy(int i)
{
    int size = WIDTH / 17;
    int offset = WIDTH / 39;
    int j=0, k=0;
    k=(i-WIDTH/297)/size;
    j=(i-WIDTH/297)%size;
    if(j<35){
        return (k*size)+offset;      
    }
    else{
        return ((k+1)*size)+offset;
    }   
}

PUBLIC int getBombHeight(Bowlingball b)
{
    return b.position.h;
}
PUBLIC int getBombWidth(Bowlingball b)
{
    return b.position.w ;
}
PUBLIC int getBombXPosition(Bowlingball b)
{
    return b.position.x;
}
PUBLIC int getBombYPosition(Bowlingball b)
{
    return b.position.y;
}

PUBLIC int BombGetPlacedBombRestriction(Bowlingball b)
{
    return b.placedBombRestriction;
}

PUBLIC int BombGetIsPlaced(Bowlingball b)
{
    return b.isPlaced;
}

PUBLIC int BombGetTimerValue(Bowlingball b)
{
    return b.timervalue;
}

PUBLIC int BombGetTimerInit(Bowlingball b)
{
    return b.timerinit;
}

PUBLIC int BombGetExplosionInit(Bowlingball b)
{
    return b.explosioninit;
}

PUBLIC void BombSetIsPlaced(Bowlingball *b, int i)
{
    b->isPlaced = i;
}

PUBLIC void BombSetXPosition(Bowlingball *b, int i)
{
    b->position.x = i;
}

PUBLIC void BombSetYPosition(Bowlingball *b, int i)
{
    b->position.y = i;
}

PUBLIC void BombSetTimerValue(Bowlingball *b, int i)
{
    b->timervalue = i;
}

PUBLIC void BombSetWhoPlacedID(Bowlingball *b, int i)
{
    b->whoPlacedID = i;
}

PUBLIC void BombSetStartvaluetimerbomb(Bowlingball *b, int i)
{
    b->startvaluetimerbomb = i;
}

PUBLIC void BombSetExplosionInit(Bowlingball *b, int i)
{
    b->explosioninit = i;
}

PUBLIC void BombSetTimerInit(Bowlingball *b, int i)
{
    b->timerinit = i;
}

PUBLIC void BombSetPlacedBombRestriction(Bowlingball *b, int i)
{
    b->placedBombRestriction = i;
}
//bomb rect possitioner i sprite sheet.
PUBLIC void loadBomb()
{
    for(int i = 0; i < 18; i++){
        bowlingballAnimation[i].w = 256;
        bowlingballAnimation[i].h = 256;
    }
    bowlingballAnimation[0].x =   0;
    bowlingballAnimation[ 0 ].y =   0;

    bowlingballAnimation[ 1 ].x = 256;
    bowlingballAnimation[ 1 ].y =   0;

    bowlingballAnimation[ 2 ].x = 512;
    bowlingballAnimation[ 2 ].y =   0;

    bowlingballAnimation[ 3 ].x = 768;
    bowlingballAnimation[ 3 ].y =   0;

    bowlingballAnimation[ 4 ].x =   0;
    bowlingballAnimation[ 4 ].y = 256;

    bowlingballAnimation[ 5 ].x = 256;
    bowlingballAnimation[ 5 ].y = 256;

    bowlingballAnimation[ 6 ].x = 512;
    bowlingballAnimation[ 6 ].y = 256;

    bowlingballAnimation[ 7 ].x = 768;
    bowlingballAnimation[ 7 ].y = 256;

    bowlingballAnimation[ 8 ].x =   0;
    bowlingballAnimation[ 8 ].y = 512;

    bowlingballAnimation[ 9 ].x = 256;
    bowlingballAnimation[ 9 ].y = 512;
    
    bowlingballAnimation[ 10 ].x = 512;
    bowlingballAnimation[ 10 ].y = 512;
    
    bowlingballAnimation[ 11 ].x = 768;
    bowlingballAnimation[ 11 ].y = 512;
    
    bowlingballAnimation[ 12 ].x =   0;
    bowlingballAnimation[ 12 ].y = 768;
    
    bowlingballAnimation[ 13 ].x = 256;
    bowlingballAnimation[ 13 ].y = 768;
    
    bowlingballAnimation[ 14 ].x = 512;
    bowlingballAnimation[ 14 ].y = 768;
    
    bowlingballAnimation[ 15 ].x = 768;
    bowlingballAnimation[ 15 ].y = 768;
    
    bowlingballAnimation[ 16 ].x =   0;
    bowlingballAnimation[ 16 ].y =1024;

    bowlingballAnimation[ 17 ].x = 256;
    bowlingballAnimation[ 17 ].y =1024;
}