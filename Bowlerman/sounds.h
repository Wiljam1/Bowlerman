#ifndef sounds_h
#define sounds_h
#include <SDL2/SDL_mixer.h>
#include "game.h"

typedef struct sounds Sounds;

Sounds initSoundFiles();
void initSounds();
void playBackroundMusic();
void playDeath(Sounds *s);
void playBomb(Sounds *s);
void destroySoundFiles();

#endif