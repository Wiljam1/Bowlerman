#include "status.h"
#include "main.h"

void initStatusLives(GameState *game)
{
    SDL_Color white = {255,255,255,255};
    SDL_Surface *tmp = TTF_RenderText_Blended(game->font, "Bowlerman!", white);
    game->labelW = tmp->w;
    game->labelH = tmp->h;
    game->label = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
}

void drawStatusLives(GameState *game)
{
    SDL_Renderer *renderer = game->renderer;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect textRect = {540, 300, game->labelW, game->labelH};
    SDL_RenderCopy(renderer, game->label, NULL, &textRect);
}

void shutdownStatusLives(GameState *game)
{
    SDL_DestroyTexture(game->label);
    game->label = NULL;
}