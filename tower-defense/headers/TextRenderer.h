#pragma once
#include "SDL.h"

class TextRenderer {
public:
	static bool init();
	static void quit();
	static void drawNumber(SDL_Renderer* renderer, int number, int tileX, int tileY, int tileSize);
};
