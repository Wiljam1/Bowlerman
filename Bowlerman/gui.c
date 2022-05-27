#include "gui.h"
#include "game.h"
#include "player.h"

#include <SDL2/SDL_ttf.h>

#define XVALUE (WIDTH/6.61)
#define YVALUE (WIDTH/238)
#define FONTSIZE (WIDTH/37.1875)

SDL_Color white = {255, 255, 255, 255};
SDL_Color blue = {142, 237, 255, 255};
SDL_Color purple = {255, 0, 255, 255};
SDL_Color red = {255, 51, 51, 255};
SDL_Color yellow = {255, 255, 51, 255};

void initGUI(Game theGame)
{
    TTF_Init(); //Init font system
    theGame->font = TTF_OpenFont("resources/fonts/abduction2002.ttf", FONTSIZE); //Set font
    theGame->updateFlag = true;
}

void updateGUI(Game theGame, Player player[])
{   
    SDL_Color playerColor;
    int id = getLocalID(theGame);
    if(id == 0)
        playerColor = blue;
    else if(id == 1)
        playerColor = purple;
    else if(id == 2)
        playerColor = red;
    else if(id == 3)
        playerColor = yellow;
    if(theGame->updateFlag == true){            //TEMPORÄR LÖSNING, FORTFARANDE MEMORY LEAK MEN TEXTEN
                                                //UPPDATERAS ENDAST NÄR ETT VÄRDE ÄNDRAS
        char tmpstr[LEN] = "Speed: ";
        createLabel(theGame, 0, tmpstr, playerGetSpeedDisplay(player, id), playerColor);  // Går att skrivas till bättre så man inte behöver hårdkoda siffror                                     
        char tmpstr2[LEN] = "Power: ";                                                         // Vet inte hur man skickar strings direkt i parametrarna (som i printf();) men det här fungerar
        createLabel(theGame, 1, tmpstr2, playerGetExplosionPower(player, id), playerColor);
        char tmpstr3[LEN] = "Bombs: ";
        createLabel(theGame, 2, tmpstr3, playerGetAmountOfBombs(player, id), playerColor); 
        char tmpstr4[LEN] = "Lives: ";
        createLabel(theGame, 3, tmpstr4, playerGetNoOfLives(player, id), playerColor); 
        char tmpstr5[LEN] = "Player 1: ";
        createLabel(theGame, 4, tmpstr5, playerGetScore(player, 0), blue);
        char tmpstr6[LEN] = "Player 2: ";
        createLabel(theGame, 5, tmpstr6, playerGetScore(player, 1), purple); 
        char tmpstr7[LEN] = "Player 3: ";
        createLabel(theGame, 6, tmpstr7, playerGetScore(player, 2), red); 
        char tmpstr8[LEN] = "Player 4: ";
        createLabel(theGame, 7, tmpstr8, playerGetScore(player, 3), yellow); 
        theGame->updateFlag = false;
    }

    // char tmpstr4[LEN] = "Lives: ";
    // createLabel(theGame, 3,tmpstr4, theGame->player[theGame->playerIDLocal].lives, white);
}

void createLabel(Game theGame, int labelID, char text[], float value, SDL_Color color)
{
    char num[LEN];

    if(value != -1){  //Don't write number if value is passed as -1
        sprintf(num, "%g", value);  //Convert float to string
        strcat(text, num);          //Add float to end of string
        free(num);
    }

    SDL_Surface *tmp = TTF_RenderText_Blended(theGame->font, text, color);
    theGame->labelW[labelID] = tmp->w;   //Save height and width to array for display
    theGame->labelH[labelID] = tmp->h;
    theGame->labels[labelID] = SDL_CreateTextureFromSurface(theGame->renderer, tmp); //Save label for rendering
    SDL_FreeSurface(tmp);

    //SDL_QueryTexture(theGame->labels[labelID], NULL, NULL, &tmp->w, &tmp->h);
}

void drawGUI(Game theGame)         // Kanske ska ändra hur de renderas i framtiden
{
    SDL_Renderer *renderer = theGame->renderer;
    //Render top row of labels
    for(int i = 0; i < TOP_ROW_LABELS; i++){                                 //MANUALLY ADJUST XXX_ROW_LABELS WHEN ADDING MORE LABELS
        SDL_Rect textRect = {i*XVALUE+(i)*50, YVALUE, theGame->labelW[i], theGame->labelH[i]}; 
        SDL_RenderCopy(renderer, theGame->labels[i], NULL, &textRect);     
    }
    //Render scores (Only renders based on amount of players joined)
    for(int i = TOP_ROW_LABELS; i < TOP_ROW_LABELS+theGame->playerAmount; i++){
        SDL_Rect textRect = {(i-4)*XVALUE+(i-4)*50, YVALUE*10, theGame->labelW[i], theGame->labelH[i]};
        SDL_RenderCopy(renderer, theGame->labels[i], NULL, &textRect);
    }
}

void destroyGUI(Game theGame)
{
    for(int i = 0; i < LABEL_AMOUNT; i++){
        SDL_DestroyTexture(theGame->labels[i]);
        theGame->labels[i] = NULL;
    }
    
    TTF_CloseFont(theGame->font);
    TTF_Quit();
}