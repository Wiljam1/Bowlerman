#include "gui.h"
#include "game.h"

#include <SDL2/SDL_ttf.h>

#define LEN 20
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

void updateGUI(Game theGame)
{   
    SDL_Color playerColor;
    if(theGame->player[theGame->playerIDLocal].id == 0)
        playerColor = blue;
    else if(theGame->player[theGame->playerIDLocal].id == 1)
        playerColor = purple;
    else if(theGame->player[theGame->playerIDLocal].id == 2)
        playerColor = red;
    else if(theGame->player[theGame->playerIDLocal].id == 3)
        playerColor = yellow;
    if(theGame->updateFlag == true){            //TEMPORÄR LÖSNING, FORTFARANDE MEMORY LEAK MEN TEXTEN
        //UPPDATERAS ENDAST NÄR ETT VÄRDE ÄNDRAS

        char tmpstr[LEN] = "Speed: ";//!!!!Ska nog göra om speed till int och dela med två någonstans så att labeln inte ändrar storlek!!!
        createLabel(theGame, 0, tmpstr, theGame->player[theGame->playerIDLocal].speedDisplay, playerColor);  // Går att skrivas till bättre så man inte behöver hårdkoda siffror                                     
        char tmpstr2[LEN] = "Power: ";                                                         // Vet inte hur man skickar strings direkt i parametrarna (som i printf();) men det här fungerar
        createLabel(theGame, 1, tmpstr2, theGame->player[theGame->playerIDLocal].explosionPower, playerColor);
        char tmpstr3[LEN] = "Bombs: ";
        createLabel(theGame, 2, tmpstr3, theGame->player[theGame->playerIDLocal].amountOfBombs, playerColor); 
        char tmpstr4[LEN] = "Lives: ";
        createLabel(theGame, 3, tmpstr4, theGame->player[theGame->playerIDLocal].noOfLives, playerColor); 
        char tmpstr5[LEN] = "Player 1: ";
        createLabel(theGame, 4, tmpstr5, theGame->player[0].score, blue);
        char tmpstr6[LEN] = "Player 2: ";
        createLabel(theGame, 5, tmpstr6, theGame->player[1].score, purple); 
        char tmpstr7[LEN] = "Player 3: ";
        createLabel(theGame, 6, tmpstr7, theGame->player[2].score, red); 
        char tmpstr8[LEN] = "Player 4: ";
        createLabel(theGame, 7, tmpstr8, theGame->player[3].score, yellow); 
        theGame->updateFlag = false;
    }

    // char tmpstr4[LEN] = "Lives: ";
    // createLabel(theGame, 3,tmpstr4, theGame->player[theGame->playerIDLocal].lives, white);
}

void createLabel(Game theGame, int labelID, char text[LEN], float value, SDL_Color color)
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
    for(int i = 0; i < TOP_ROW_LABELS; i++){                                 //MANUALLY ADJUST X_ROW_LABELS WHEN ADDING MORE LABELS
        SDL_Rect textRect = {i*XVALUE+(i)*50, YVALUE, theGame->labelW[i], theGame->labelH[i]}; 
        SDL_RenderCopy(renderer, theGame->labels[i], NULL, &textRect);     
    }
    //Render bottom row of labels (could be score for every player in the future)
    for(int i = TOP_ROW_LABELS; i < BOTTOM_ROW_LABELS; i++){
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