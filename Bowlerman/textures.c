#include "textures.h"
#include "game.h"
#include "player.h"
#include "bomb.h"
#include "collisionDetection.h"
#include "wall.h"
#include "powerup.h"
#include "gui.h"
#include <string.h>

PUBLIC SDL_Texture *loadTextures(Game newGame, char fileLocation[]) // loadmedia
{
    bool success = true;
    char fileLocationInResources[100] = "resources/";
    strcat(fileLocationInResources, fileLocation);
    newGame->window_surface = IMG_Load(fileLocationInResources);
    if (newGame->window_surface == NULL)
    {
        printf("Failed to load surface! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    return SDL_CreateTextureFromSurface(newGame->renderer, newGame->window_surface);
}

void renderTextures(Game theGame, Player player[])
{
    // Define stuff to make function easier to read
    SDL_Renderer *renderer = theGame->renderer;
    int id = theGame->playerIDLocal;

    // clear renderer
    SDL_RenderClear(renderer);

    // updates/renders background
    SDL_Rect backRect = {0, (WIDTH/11.9), WIDTH, HEIGHT};
    SDL_RenderCopy(renderer, theGame->background, NULL, &backRect);

    renderPowerups(theGame);

    renderWalls(theGame);

    // render bombs and explosion
    int returnarray[20]={0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3};
    for (int i = 0; i < MAXBOMBAMOUNT; i++)
    {
        if (theGame->bombs[i].timervalue == 0)
        {
            SDL_RenderCopy(renderer, theGame->bomb_texture[returnarray[i]], &bowlingballAnimation[0], &theGame->bombs[i].position);
        }
        if (theGame->bombs[i].explosioninit == 0)
        {
            for (int j = 0; j < 1+4*playerGetExplosionPower(player, returnarray[i]); j++)
            {
                if(testCollisionExplosionWithWalls(theGame, j) == 0)
                {
                    SDL_RenderCopy(theGame->renderer, theGame->bombExplosion_texture, &bowlingballAnimation[0], &theGame->explosionPosition[i][j]);
                }
            }
        }
    }

    UpdatePlayerTextures(theGame, player);
    
    // Draw GUI last (top of screenlayers)
    drawGUI(theGame);

    SDL_RenderPresent(renderer); // present renderer
}

void renderPowerups(Game theGame)
{
    // //Draw powerups
    for(int i = 0; i < POWERUPAMOUNT; i++){
        if(theGame->powerups[i].isPickedUp == false) //Draw until picked up
        {
            SDL_Rect powerupRect = {theGame->powerups[i].x, theGame->powerups[i].y, 
                                    theGame->powerups[i].w, theGame->powerups[i].h};
        
            SDL_RenderCopy(theGame->renderer, theGame->texturePowerups[theGame->powerups[i].type], NULL, &powerupRect); 
        }
    }
}

PUBLIC void loadAllTextures(Game theGame)
{

     // loads in textures
    theGame->background = (SDL_Texture *)loadTextures(theGame, "alley.png");
    /*PLAYER ID 0*/
    theGame->player_texture[0][0] = (SDL_Texture *)loadTextures(theGame, "blueman/down.png");
    theGame->player_texture[0][1] = (SDL_Texture *)loadTextures(theGame, "blueman/up.png");
    theGame->player_texture[0][2] = (SDL_Texture *)loadTextures(theGame, "blueman/right.png");
    theGame->player_texture[0][3] = (SDL_Texture *)loadTextures(theGame, "blueman/left.png");
    /*PLAYER ID 1*/
    theGame->player_texture[1][0] = (SDL_Texture *)loadTextures(theGame, "purpleman/down.png");
    theGame->player_texture[1][1] = (SDL_Texture *)loadTextures(theGame, "purpleman/up.png");
    theGame->player_texture[1][2] = (SDL_Texture *)loadTextures(theGame, "purpleman/right.png");
    theGame->player_texture[1][3] = (SDL_Texture *)loadTextures(theGame, "purpleman/left.png");
    /*PLAYER ID 2*/
    theGame->player_texture[2][0] = (SDL_Texture *)loadTextures(theGame, "redman/down.png");
    theGame->player_texture[2][1] = (SDL_Texture *)loadTextures(theGame, "redman/up.png");
    theGame->player_texture[2][2] = (SDL_Texture *)loadTextures(theGame, "redman/right.png");
    theGame->player_texture[2][3] = (SDL_Texture *)loadTextures(theGame, "redman/left.png");
    /*PLAYER ID 3*/
    theGame->player_texture[3][0] = (SDL_Texture *)loadTextures(theGame, "yellowman/down.png");
    theGame->player_texture[3][1] = (SDL_Texture *)loadTextures(theGame, "yellowman/up.png");
    theGame->player_texture[3][2] = (SDL_Texture *)loadTextures(theGame, "yellowman/right.png");
    theGame->player_texture[3][3] = (SDL_Texture *)loadTextures(theGame, "yellowman/left.png");
    /*BOWLING BALLS*/
    theGame->bomb_texture[0] = (SDL_Texture *)loadTextures(theGame, "Bowling_Ball_BLue.png");
    theGame->bomb_texture[1] = (SDL_Texture *)loadTextures(theGame, "Bowling_Ball_Purple.png");
    theGame->bomb_texture[2] = (SDL_Texture *)loadTextures(theGame, "Bowling_Ball_Red.png");
    theGame->bomb_texture[3] = (SDL_Texture *)loadTextures(theGame, "Bowling_Ball_Yellow.png");
    theGame->bombExplosion_texture = (SDL_Texture *)loadTextures(theGame, "FIRE.png");
    /*WALLS*/
    theGame->textureWall[0] = (SDL_Texture *)loadTextures(theGame, "walls/40/straight.png");
    theGame->textureWall[1] = (SDL_Texture *)loadTextures(theGame, "walls/40/corner.png");
    theGame->textureWall[2] = (SDL_Texture *)loadTextures(theGame, "walls/40/center.png");
    theGame->textureWall[3] = (SDL_Texture *)loadTextures(theGame, "wall.png");
    /*POWERUPS*/
    theGame->texturePowerups[0] = (SDL_Texture *)loadTextures(theGame, "speedPowerup.png");
    theGame->texturePowerups[1] = (SDL_Texture *)loadTextures(theGame, "explosionPowerPowerup.png");
    theGame->texturePowerups[2] = (SDL_Texture *)loadTextures(theGame, "moreBombsPowerup.png");
    //theGame->texturePowerups[3] = (SDL_Texture *)loadTextures(theGame, "canRollBombsPowerup.png");
    /*Menu*/
    theGame->menuBackground = (SDL_Texture *)loadTextures(theGame, "menu/menuBackground.png");
    theGame->logo = (SDL_Texture *)loadTextures(theGame, "menu/logo.png");
    theGame->menuButtons[0] = (SDL_Texture *)loadTextures(theGame, "menu/play.png");
    theGame->menuButtons[1] = (SDL_Texture *)loadTextures(theGame, "menu/host.png");
    theGame->menuButtons[2] = (SDL_Texture *)loadTextures(theGame, "menu/join.png");
    theGame->menuButtons[3] = (SDL_Texture *)loadTextures(theGame, "menu/start.png");
    theGame->menuButtons[4] = (SDL_Texture *)loadTextures(theGame, "menu/quit.png");
    theGame->menuButtons[5] = (SDL_Texture *)loadTextures(theGame, "menu/backArrow.png");
    theGame->menuButtons[6] = (SDL_Texture *)loadTextures(theGame, "menu/textfield.png");

    SDL_FreeSurface(theGame->window_surface);
    // Load player sprites
    theGame->pSprites = GetPlayerSprite();
    // Load bomb sprites
    loadBomb();
}