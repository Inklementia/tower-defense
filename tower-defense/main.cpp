#include "Game.h"
#include "GameConfig.h"
#include "HudRenderer.h"
#include "LevelSettings.h"
#include "Menu.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include "TextureLoader.h"
#include <ctime>
#include <iostream>

int main(int argc, char* args[]) {
	//Seed the random number generator with the current time so that it will generate different 
	//numbers every time the game is run.
	srand((unsigned)time(NULL));

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::cout << "Error: Couldn't initialize SDL Video or Audio = " << SDL_GetError() << std::endl;
		return 1;
	}
	else {
		//Setup the audio mixer.
		bool isSDLMixerLoaded = (Mix_OpenAudio(
			GameConfig::AUDIO_FREQUENCY,
			MIX_DEFAULT_FORMAT,
			GameConfig::AUDIO_CHANNELS,
			GameConfig::AUDIO_CHUNK_SIZE) == 0);
		if (isSDLMixerLoaded == false) {
			std::cout << "Error: Couldn't initialize Mix_OpenAudio = " << Mix_GetError() << std::endl;
		}
		else {
			Mix_AllocateChannels(GameConfig::AUDIO_MIXER_CHANNELS);

			//Output the name of the audio driver.
			std::cout << "Audio driver = " << SDL_GetCurrentAudioDriver() << std::endl;
		}


		//Create the window.
		SDL_Window* window = SDL_CreateWindow("Tower Base Defense",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			GameConfig::WINDOW_WIDTH, GameConfig::WINDOW_HEIGHT, 0);
		if (window == nullptr) {
			std::cout << "Error: Couldn't create window = " << SDL_GetError() << std::endl;
			return 1;
		}
		else {
			//Create a renderer for GPU accelerated drawing.
			SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
				SDL_RENDERER_PRESENTVSYNC);
			if (renderer == nullptr) {
				std::cout << "Error: Couldn't create renderer = " << SDL_GetError() << std::endl;
				return 1;
			}
			else {
				//Ensure transparent graphics are drawn correctly.
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

				if (!HudRenderer::init()) {
					std::cout << "Warning: Could not load HUD font" << std::endl;
				}

				//Output the name of the render driver.
				SDL_RendererInfo rendererInfo;
				SDL_GetRendererInfo(renderer, &rendererInfo);
				std::cout << "Renderer = " << rendererInfo.name << std::endl;

				bool appRunning = true;
				while (appRunning) {
					int levelIndex = Menu::run(renderer);
					if (levelIndex < 0) {
						appRunning = false;
						break;
					}

					LevelSettings settings = LevelLoader::load(LevelLoader::getLevelPath(levelIndex));
					Game game(renderer, settings);
					GameRunResult result = game.run();

					if (result == GameRunResult::quit)
						appRunning = false;
				}

				TextureLoader::deallocateTextures();
				HudRenderer::shutdown();

				//Clean up.
				SDL_DestroyRenderer(renderer);
			}

			//Clean up.
			SDL_DestroyWindow(window);
		}

		//Clean up.
		if (isSDLMixerLoaded) {
			Mix_CloseAudio();
			Mix_Quit();
		}

		SDL_Quit();
	}
	return 0;
}