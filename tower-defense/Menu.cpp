#include "Menu.h"
#include "GameConfig.h"
#include "HudRenderer.h"

namespace {
struct MenuEntry {
	const char* label;
};

const MenuEntry MENU_ENTRIES[2] = {
	{"Level 1"},
	{"Creative"},
};
}

int Menu::run(SDL_Renderer* renderer) {
	bool running = true;
	int selectedLevel = -1;

	while (running) {
		int mouseX = 0, mouseY = 0;
		SDL_GetMouseState(&mouseX, &mouseY);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				return -1;
			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_ESCAPE:
					return -1;
				case SDL_SCANCODE_1:
					selectedLevel = 0;
					running = false;
					break;
				case SDL_SCANCODE_2:
					selectedLevel = 1;
					running = false;
					break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					for (int i = 0; i < 2; i++) {
						int bx = (GameConfig::WINDOW_WIDTH - 440) / 2;
						int by = 180 + i * (64 + 14);
						if (event.button.x >= bx && event.button.x <= bx + 440 &&
							event.button.y >= by && event.button.y <= by + 64) {
							selectedLevel = i;
							running = false;
							break;
						}
					}
				}
				break;
			}
		}

		HudRenderer::drawCheckerboard(renderer, GameConfig::WINDOW_WIDTH, GameConfig::WINDOW_HEIGHT,
			GameConfig::TILE_SIZE, UiColor{46, 46, 52, 255}, UiColor{32, 32, 36, 255});

		HudRenderer::drawTextCentered(renderer, GameConfig::WINDOW_WIDTH / 2, 48,
			"Minimalistic Tower Defense", UiColor{255, 196, 64, 255}, 42);

		for (int i = 0; i < 2; i++) {
			int bx = (GameConfig::WINDOW_WIDTH - 440) / 2;
			int by = 180 + i * (64 + 14);
			bool hovered = mouseX >= bx && mouseX <= bx + 440 &&
				mouseY >= by && mouseY <= by + 64;

			HudRenderer::drawPanel(renderer, bx, by, 440, 64,
				hovered ? UiColor{48, 54, 72, 255} : UiColor{36, 40, 54, 255});
			HudRenderer::drawPanel(renderer, bx, by, 440, 3, UiColor{255, 196, 64, 255});
			HudRenderer::drawText(renderer, bx + 20, by + 20, MENU_ENTRIES[i].label,
				UiColor{235, 235, 240, 255}, 24);
		}

		HudRenderer::drawTextCentered(renderer, GameConfig::WINDOW_WIDTH / 2, 420,
			"Press 1 or 2, or click a mode", UiColor{150, 154, 168, 255}, 16);
		HudRenderer::drawTextCentered(renderer, GameConfig::WINDOW_WIDTH / 2, 442,
			"ESC to quit", UiColor{150, 154, 168, 255}, 16);

		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}

	return selectedLevel;
}
