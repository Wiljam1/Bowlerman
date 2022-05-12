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
#define BOMBTIMER 3000


PUBLIC Bowlingball initBomb();
void sortBombsArray(Game theGame,int i);


PUBLIC Bowlingball initBomb()
{
    //malloc(sizeof(struct bowling_ball));
    Bowlingball b;
    b.position.y = 0;
    b.position.x = 0;
    b.position.w = 70;
    b.position.h = 70;
    b.speed = 0;
    b.timervalue = 0;           //också en initiering för bomberna
    b.timerinit = 1;            //initierar timer för bomber
    b.explosioninit = 1;        // initierar explosionerna
    b.placedBombRestriction = 1; //gör så man inte kan lägga en bomb samtidigt som en är ute             
    b.isPlaced = 1;
    b.startvaluetimer = 0;
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
    amount = theGame->player[playerID].amountOfBombsPlaced*4;

    if (theGame->player[playerID].amountOfBombsPlaced < theGame->player[playerID].amountOfBombs) // man måste veta vilken player här
    {
        theGame->bombs[playerID+amount] = initBomb();
        theGame->bombs[playerID+amount].position.y = correctBowlingBallPosy(getPlayerYPosition(theGame->player[playerID]));
        theGame->bombs[playerID+amount].position.x = correctBowlingBallPosx(getPlayerXPosition(theGame->player[playerID]));
        theGame->bombs[playerID+amount].timervalue = initbowlingballtimer(SDL_GetTicks(), BOMBTIMER, playerID+amount);
        theGame->player[playerID].amountOfBombsPlaced++;                //antal bomber som är placerade
        theGame->bombs[playerID+amount].startvaluetimer = SDL_GetTicks();
    }
}

//som en game loop för bomber, kollar timer för explosioner samt bomber
void process(Game theGame, Sounds *s)
{
    //kollar bombernas timer, är den klar försvinner bomben och explosionstimer initieras
    for (int i = 0; i < MAXBOMBAMOUNT; i++){
        if (theGame->bombs[i].timerinit == 1){
                  
            if (theGame->bombs[i].timervalue == 1){
                initExplosionPosition(theGame, i);
                initbowlingballtimer(SDL_GetTicks(), 1000, i);
                testPossibilityToExplodeWithBombs(theGame, i);
                theGame->bombs[i].timerinit = 0;
                theGame->bombs[i].explosioninit = 0;
                playBomb(s);
            }
            else {
                theGame->bombs[i].timervalue = initbowlingballtimer(0, BOMBTIMER, i);
            }
             
        }
    }
    //kollar explosionstimern
    for (int i = 0; i < MAXBOMBAMOUNT; i++){
        if (theGame->bombs[i].explosioninit == 0){
            theGame->bombs[i].explosioninit = initbowlingballtimer(0, 1000, i);
            for(int j=139;j<250;j++){
                if(theGame->wall[j].destroyedWall == 0){
                    theGame->wall[j].destroyedWall = testCollisionWithDestroyableWalls(theGame, j, i);
                    if(theGame->wall[j].destroyedWall){ //If wall is destroyed...
                        rollForPowerup(theGame, theGame->wall[j].x, theGame->wall[j].y);        //kallar på powerupp
                    }
                }
            }
            if (theGame->bombs[i].explosioninit == 1){
                int returnarray[20]={0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3};
                theGame->bombs[i].isPlaced = 0;
                sortBombsArray(theGame, returnarray[i]);
                if(theGame->player[returnarray[i]].amountOfBombsPlaced > 0)
                {
                    theGame->player[returnarray[i]].amountOfBombsPlaced--;          
                }
            }
        }
    }
}

//sorterar bomber så att sprängda bomber åker iväg, funkar inte helt med timer dock
void sortBombsArray(Game theGame,int i)
{
    Bowlingball tmp;
    int temporarytime=0;
    //for (int k=0;k<theGame->player[i].amountOfBombsPlaced;k++)
    {
        for (int j = 1;j<theGame->player[i].amountOfBombsPlaced;j++)
        {
            if(theGame->bombs[i+j*4].isPlaced == 1)
            {
                if(theGame->bombs[i+j*4-4].isPlaced == 0)
                {
                    tmp = theGame->bombs[i+j*4-4];
                    theGame->bombs[i+j*4-4] = theGame->bombs[i+j*4];
                    theGame->bombs[i+j*4] = tmp;
                    initbowlingballtimer(theGame->bombs[i+j*4].startvaluetimer, BOMBTIMER, i+j*4-4);
                }
            }
        }  
    }
}

//sätter positionerna för explosionen efter vart bomben legat
void initExplosionPosition(Game theGame, int playerID)
{
    int tilesize = 66, diff=2; //Borde sparas i en struct för att komma åt värdet vid collisiondetection?
    int returnarray[20]={0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3};
    SDL_Rect temp = {0,0,tilesize,tilesize};
    for(int i = 0; i < 1+4*theGame->player[returnarray[playerID]].explosionPower; i++){
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
    for(int i=1;i<theGame->player[returnarray[playerID]].explosionPower+1;i++)
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
    
    for (int i=1;i<1+4*theGame->player[returnarray[playerID]].explosionPower;i++)
    {
        if (testPossibilityToExplode(theGame, playerID, i) == 0)        //för oförstörbara väggar
        {
            for(int j=0;j<theGame->player[returnarray[playerID]].explosionPower;j++)
            {
                theGame->explosionPosition[playerID][i+4*j].h = 0;
                theGame->explosionPosition[playerID][i+4*j].w = 0;
                theGame->explosionPosition[playerID][i+4*j].y = 2000;
                theGame->explosionPosition[playerID][i+4*j].x = 2000;
            }
        }
        if (testPossibilityToExplodeDestroyableWalls(theGame, playerID, i) == 0)    //för förstörbara väggar
        {
            for(int j=1;j<theGame->player[returnarray[playerID]].explosionPower;j++)
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
    int j=0, k=0;
    i -= 12;
    k=i/70;
    j=i%70;
    if(j<35){
        return (k*70);      //returnerar närmaste tile sen +3 för att få det helt centrerat
    }
    else{
        return ((k+1)*70);
    }   
}


int correctBowlingBallPosy(int i)
{
    int j=0, k=0;
    k=(i-4)/70;
    j=(i-4)%70;
    if(j<35){
        return (k*70)+30;      //returnerar närmaste tile sen +3 för att få det helt centrerat
    }
    else{
        return ((k+1)*70)+30;
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