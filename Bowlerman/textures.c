#include "textures.h"
#include "game.h"


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
    theGame->textureWall[0] = (SDL_Texture *)loadTextures(theGame, "walls/80/straight.png");
    theGame->textureWall[1] = (SDL_Texture *)loadTextures(theGame, "walls/80/corner.png");
    theGame->textureWall[2] = (SDL_Texture *)loadTextures(theGame, "walls/80/center.png");
    theGame->textureWall[3] = (SDL_Texture *)loadTextures(theGame, "wall.png");

    SDL_FreeSurface(theGame->window_surface);
    // Load player sprites
    theGame->pSprites = GetPlayerSprite();
    // Load bomb sprites
    loadBomb();
}