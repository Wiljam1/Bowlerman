#ifndef sounds_h
#define sounds_h
#include <SDL2/SDL_mixer.h>
#include "game.h"

struct sounds
{
    Mix_Music *backgroundMusic;
    Mix_Chunk *explosion;
};
typedef struct sounds *Sounds;

Sounds initSoundFiles();
void initSounds();
void playBackroundMusic();
void playExplosion();
void destroySoundFiles();

#endif