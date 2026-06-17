#pragma once
#include "SDL.h"

class Menu {
public:
	//Returns level index 0-1, or -1 to quit the application.
	static int run(SDL_Renderer* renderer);
};
