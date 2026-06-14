#pragma once

namespace GameConfig {
	constexpr int WINDOW_WIDTH = 720;
	constexpr int WINDOW_HEIGHT = 720;

	constexpr int TILE_SIZE = 48;

	constexpr int TILE_COUNT_X = WINDOW_WIDTH / TILE_SIZE;
	constexpr int TILE_COUNT_Y = WINDOW_HEIGHT / TILE_SIZE;
}
