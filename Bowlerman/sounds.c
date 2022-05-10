#include <stdio.h>
#include <SDL2/SDL_mixer.h>
#include "sounds.h"
#include "game.h"

/*Ändra här ifall du tröttnar på ljudet*/
#define MUSICVOLUME 1
#define BOMBVOLUME 1

Sounds initSoundFiles()
{
    Sounds s = malloc(sizeof(Sounds));
    s->backgroundMusic = Mix_LoadMUS("resources/sounds/song.wav");
    s->explosion = Mix_LoadWAV("resources/sounds/explosion.wav");

    return s;
}

void initSounds()
{
    if (Mix_OpenAudio(24100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("Error: %s", Mix_GetError());
    }
}

void playBackroundMusic(Sounds s)
{   
    if(!Mix_PlayingMusic())
    {
        Mix_VolumeMusic(MUSICVOLUME);
        Mix_PlayMusic(s->backgroundMusic, -1);
    }
}

void playExplosion(Sounds s)
{
    Mix_VolumeChunk(s->explosion, BOMBVOLUME);
    Mix_PlayChannel(-1, (s->explosion), 0);
}

void destroySoundFiles(Sounds s)
{
    Mix_FreeMusic(s->backgroundMusic);
    Mix_FreeChunk(s->explosion);
}