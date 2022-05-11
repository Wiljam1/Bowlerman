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
    //Speed label
    SDL_Color white = {255, 255, 255, 255};
    char tmpstr[20] = "Speed: ";
    float value = theGame->player[theGame->playerIDLocal].speed;
    char num[5];

    sprintf(num, "%g", value); // Convert float to string
    strcat(tmpstr, num); // Cat string together
    free(num); 

    SDL_Surface *temp = TTF_RenderText_Blended(theGame->font, tmpstr, white);
    theGame->labelW = temp->w;
    theGame->labelH = temp->h;
    theGame->labels[0] = SDL_CreateTextureFromSurface(theGame->renderer, temp);
    SDL_FreeSurface(temp);
    // --------------------------------------------------------------------
    //Explosionpower label                                                              //SKA GÖRA DET HÄR TILL EN FUNKTION IMORN
    //SDL_Color white = {255, 255, 255, 255};
    char tmpstr2[20] = "Power: ";
    int value2 = theGame->player[theGame->playerIDLocal].explosionPower;
    char num2[5];

    sprintf(num2, "%d", value2); // Convert int to string
    strcat(tmpstr2, num2); // Cat string together
    free(num2); 

    SDL_Surface *temp2 = TTF_RenderText_Blended(theGame->font, tmpstr2, white);
    theGame->labelW = temp->w;
    theGame->labelH = temp->h;
    theGame->labels[1] = SDL_CreateTextureFromSurface(theGame->renderer, temp2);
    SDL_FreeSurface(temp2);
    // --------------------------------------------------------------------
}

void drawGUI(Game theGame)
{
    SDL_Renderer *renderer = theGame->renderer;
    
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    for(int i = 0; i < 2; i++){
        SDL_Rect textRect = {i*150, 0, theGame->labelW, theGame->labelH};
        SDL_RenderCopy(renderer, theGame->labels[i], NULL, &textRect);
    }
}

void destroyGUI(Game theGame)
{
    SDL_DestroyTexture(theGame->labels[0]);
    theGame->labels[0] = NULL;
    TTF_CloseFont(theGame->font);
    TTF_Quit();
}