#include "includes.h"

// Sounds Library

// SoundEffect

// Construct Sound Effect
void Sound_constructSoundEffect(SoundEffect *soundEffect, const char *filename)
{
  soundEffect->sdlChunk = Mix_LoadWAV(filename);
  soundEffect->filename = filename;
  soundEffect->duration = soundEffect->sdlChunk->alen;
  soundEffect->channel = -1;

  // Statistics
  soundEffect->currentChannelInstances = 0;
  soundEffect->totalChannelInstances = 0;
}

void Sound_deconstructSoundEffect(SoundEffect *soundEffect)
{
  Mix_FreeChunk(soundEffect->sdlChunk);
}

// Play Sound Effect
int Sound_playSoundEffect(SoundEffect *soundEffect, int loops, int channel, int volume, bool useSameChannel)
{
  if (useSameChannel)
  {
    // use existing channel if soundEffect->channel is set
    if (soundEffect->channel > -1)
    {
      Mix_HaltChannel(soundEffect->channel); // clear channel sounds
      Mix_PlayChannel(soundEffect->channel, soundEffect->sdlChunk, loops); // play new sound
    }
    // find channel if needed, and set soundEffect to returned channel
    else
    {
      soundEffect->channel = Mix_PlayChannel(-1, soundEffect->sdlChunk, loops);
    }
  }
  else
  {
    soundEffect->channel = Mix_PlayChannel(channel, soundEffect->sdlChunk, loops);
  }
  printf("channel: %d\n", soundEffect->channel);
  if (soundEffect->channel == -1)
  {
    printf("Mix_PlayChannel(-1, soundEffect->sdlChunk) screwed up: %s \n", Mix_GetError());

    return -5;
  }
  return soundEffect->channel;
}

// Stop Sound Effect
void Sound_pauseSoundEffect(SoundEffect *soundEffect)
{
  Mix_Pause(soundEffect->channel);
}

// MusicEffect
void Sound_constructMusicEffect(MusicEffect *musicEffect, const char *filename, int volume)
{
  musicEffect->sdlMusic = Mix_LoadMUS(filename);
  Mix_VolumeMusic(volume);
  Mix_PlayMusic(musicEffect->sdlMusic, -1);
}

// Change Music Effect
void Sound_changeMusic(MusicEffect *musicEffect, const char *filename)
{
  musicEffect->sdlMusic = Mix_LoadMUS(filename);
}

// Pause Music Effect
void Sound_pauseMusic()
{
  Mix_PauseMusic();
}
