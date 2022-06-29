#pragma once

typedef struct SoundEffect SoundEffect;
typedef struct MusicEffect MusicEffect;

struct SoundEffect
{
  Mix_Chunk *sdlChunk;
  const char *filename;
  int channel;
  int duration;
  int startTick;

  // Statistics
  int totalChannelInstances;
  int currentChannelInstances;
};

struct MusicEffect
{
  Mix_Music *sdlMusic;
  int startTick;
  int duration;
};

// Construct Sound Effect
void Sound_constructSoundEffect(SoundEffect *soundEffect, const char *filename);

// Deconstruct Sound Effect
void Sound_deconstructSoundEffect(SoundEffect *soundEffect);

// Play Sound Effect
int Sound_playSoundEffect(SoundEffect *soundEffect, int loops, int channel, int volume, bool useSameChannel);

// Stop Sound Effect
void Sound_pauseSoundEffect(SoundEffect *soundEffect);

// Contruct Music Effect
void Sound_constructMusicEffect(MusicEffect *musicEffect, const char *filename, int volume);

// Change Music Effect
void Sound_changeMusic(MusicEffect *musicEffect, const char *filename);

// Pause Music Effect
void Sound_pauseMusic();
