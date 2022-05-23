#include <stdio.h>
#include <SDL2/SDL_mixer.h>
#include "sounds.h"
#include "game.h"

/*Ändra här ifall du tröttnar på ljudet*/
#define MUSICVOLUME 1
#define BOMBVOLUME 1
#define STRIKEVOLUME 1

Sounds initSoundFiles()
{
    Sounds s;
    s.backgroundMusic = Mix_LoadMUS("resources/sounds/song.wav");
    s.explosion = Mix_LoadWAV("resources/sounds/explosion.wav");
    s.death = Mix_LoadWAV("resources/sounds/strike.wav");
    Mix_Volume(-1, 1);
    Mix_VolumeMusic(MUSICVOLUME);
    
    return s;
}

void muteOrStartMusic()
{
    static int play = 1;

    if (!play) {
        Mix_VolumeMusic(MUSICVOLUME);
        play = 1;
    } else {
        Mix_VolumeMusic(0);
        play = 0;
    }
}

void initSounds()
{
    if (Mix_OpenAudio(24100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("Error: %s", Mix_GetError());
    }
}

void playBackroundMusic(Sounds *s)
{   
    if(!Mix_PlayingMusic())
    {
        Mix_PlayMusic(s->backgroundMusic, -1);
    }
}

void playDeath(Sounds *s)
{
    Mix_PlayChannel(-1, (s->death), 0);
}

void playBomb(Sounds *s)
{
    Mix_PlayChannel(-1, (s->explosion), 0);
}

void destroySoundFiles(Sounds *s)
{
    Mix_FreeMusic(s->backgroundMusic);
    Mix_FreeChunk(s->explosion);
    Mix_FreeChunk(s->death);
}