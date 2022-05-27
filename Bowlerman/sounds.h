#ifndef sounds_h
#define sounds_h
#include <SDL2/SDL_mixer.h>
#include "game.h"

// Struct för ljudfiler
typedef struct sounds *Sounds;

//getting sound files
Sounds initSoundFiles();
//initialize sounds
void initSounds();
//Starting background music
void playBackroundMusic(Sounds s);
//Sound to play when dying
void playDeath(Sounds s);
// Sound to play when bomb explodes
void playBomb(Sounds s);
// Remove sounds from memory
void destroySoundFiles();
void toggleMusic();

#endif