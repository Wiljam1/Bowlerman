#include "gui.h"
#include "game.h"

#include <SDL2/SDL_ttf.h>

#define LEN 20
#define XVALUE (WIDTH/6.61)
#define YVALUE (WIDTH/238)
#define FONTSIZE (WIDTH/37.1875)

void initGUI(Game theGame)
{
    TTF_Init(); //Init font system
    theGame->font = TTF_OpenFont("resources/fonts/abduction2002.ttf", FONTSIZE); //Set font
    theGame->updateFlag = true;
}

void updateGUI(Game theGame)
{   
    if(theGame->updateFlag == true){            //TEMPORÄR LÖSNING, FORTFARANDE MEMORY LEAK MEN TEXTEN
        SDL_Color white = {255, 255, 255, 255}; //UPPDATERAS ENDAST NÄR ETT VÄRDE ÄNDRAS

        char tmpstr[LEN] = "Speed: ";//!!!!Ska nog göra om speed till int och dela med två någonstans så att labeln inte ändrar storlek!!!
        createLabel(theGame, 0, tmpstr, theGame->player[theGame->playerIDLocal].speed, white);  // Går att skrivas till bättre så man inte behöver hårdkoda siffror                                     
        char tmpstr2[LEN] = "Power: ";                                                         // Vet inte hur man skickar strings direkt i parametrarna (som i printf();) men det här fungerar
        createLabel(theGame, 1, tmpstr2, theGame->player[theGame->playerIDLocal].explosionPower, white);
        char tmpstr3[LEN] = "Bombs: ";
        createLabel(theGame, 2, tmpstr3, theGame->player[theGame->playerIDLocal].amountOfBombs, white); 
        char tmpstr4[LEN] = "Lives: ";
        createLabel(theGame, 3, tmpstr4, theGame->player[theGame->playerIDLocal].noOfLives, white); 
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
    theGame->labelW = tmp->w;   //Save height and width to struct
    theGame->labelH = tmp->h;
    theGame->labels[labelID] = SDL_CreateTextureFromSurface(theGame->renderer, tmp); //Save label for rendering
    SDL_FreeSurface(tmp);

    //SDL_QueryTexture(theGame->labels[labelID], NULL, NULL, &tmp->w, &tmp->h);
}

void drawGUI(Game theGame)         // Kanske ska ändra hur de renderas i framtiden
{
    SDL_Renderer *renderer = theGame->renderer;
    //Render top row of labels
    for(int i = 0; i < LABELAMOUNT; i++){                                 //MANUALLY ADJUST LABELAMOUNT WHEN ADDING MORE LABELS
        SDL_Rect textRect = {i*XVALUE, YVALUE, theGame->labelW, theGame->labelH}; 
        SDL_RenderCopy(renderer, theGame->labels[i], NULL, &textRect);     
    }
    //Render bottom row of labels (could be score for every player in the future)

}

void destroyGUI(Game theGame)
{
    for(int i = 0; i < LABELAMOUNT; i++){
        SDL_DestroyTexture(theGame->labels[i]);
        theGame->labels[i] = NULL;
    }
    
    TTF_CloseFont(theGame->font);
    TTF_Quit();
}