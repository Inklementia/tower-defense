#pragma once
#include "SDL.h"
#include <string>
#include <unordered_map>


class TextureLoader {
public:
  static SDL_Texture *loadTexture(SDL_Renderer *renderer, std::string filename);
  static void deallocateTextures();

private:
  static std::unordered_map<std::string, SDL_Texture *> umapTexturesLoaded;
};