#include "gui.h"
#include "game.h"

#include <SDL2/SDL_ttf.h>


void initGUI(Game theGame)
{
    TTF_Init(); //Init font system
    theGame->font = TTF_OpenFont("resources/fonts/Restoe Iboe.ttf", 32);

}

void updateGUI(Game theGame)
{
    //Make a label
    SDL_Color white = {255, 255, 255, 255};
    char tmpstr[9] = "Speed: ";
    float speedValue = theGame->player[theGame->playerIDLocal].speed;
    char num[5];

    sprintf(num, "%g", speedValue); // Convert float to string
    strcat(tmpstr, num); // Cat string together
    free(num); 

    SDL_Surface *temp = TTF_RenderText_Blended(theGame->font, tmpstr, white);
    theGame->labelW = temp->w;
    theGame->labelH = temp->h;
    theGame->label = SDL_CreateTextureFromSurface(theGame->renderer, temp);
    SDL_FreeSurface(temp);
    // --------------------------------------------------------------------
}

void drawGUI(Game theGame)
{
    SDL_Renderer *renderer = theGame->renderer;
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    SDL_Rect textRect = {0, 0, theGame->labelW, theGame->labelH};
    SDL_RenderCopy(renderer, theGame->label, NULL, &textRect);
}

void destroyGUI(Game theGame)
{
    SDL_DestroyTexture(theGame->label);
    theGame->label = NULL;
    TTF_CloseFont(theGame->font);
    TTF_Quit();
}