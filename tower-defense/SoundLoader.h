#pragma once
#include "SDL_mixer.h"
#include <string>
#include <unordered_map>


class SoundLoader {
public:
  static Mix_Chunk *loadSound(std::string filename);
  static void deallocateSounds();

private:
  static std::unordered_map<std::string, Mix_Chunk *> umapSoundsLoaded;
};