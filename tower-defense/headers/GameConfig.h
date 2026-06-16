#pragma once

namespace GameConfig {
	// Window
	constexpr int WINDOW_WIDTH = 960;
	constexpr int WINDOW_HEIGHT = 576;

	// Grid
	constexpr int TILE_SIZE = 64;
	constexpr int TILE_COUNT_X = WINDOW_WIDTH / TILE_SIZE;
	constexpr int TILE_COUNT_Y = WINDOW_HEIGHT / TILE_SIZE;
	constexpr float TILE_CENTER_OFFSET = 0.5f;

	// Game loop
	constexpr int FPS = 60;
	constexpr float FRAME_DT = 1.0f / FPS;

	// Spawning
	constexpr float SPAWN_INTERVAL_S = 0.25f;
	constexpr float ROUND_DELAY_S = 5.0f;
	constexpr int UNITS_PER_ROUND = 15;

	// Units
	constexpr float UNIT_SPEED = 1.5f;
	constexpr float UNIT_SIZE = 0.4f;
	constexpr float UNIT_WALL_SPACING = 0.35f;
	constexpr float UNIT_TARGET_REACH_DISTANCE = 0.5f;
	constexpr float UNIT_OVERLAP_MIN_DISTANCE = 0.01f;
	constexpr float PI = 3.14159265359f;
	constexpr float UNIT_COLLISION_ANGLE = PI / 4.0f;

	// Turrets
	constexpr float TURRET_ROTATION_SPEED_DEG = 180.0f;
	constexpr float TURRET_WEAPON_RANGE = 5.0f;

	// Level / flow field
	constexpr unsigned char FLOW_DISTANCE_MAX = 255;

	// UI
	constexpr int OVERLAY_X = 40;
	constexpr int OVERLAY_Y = 40;

	// Colors (RGBA)
	constexpr int CLEAR_COLOR_R = 255;
	constexpr int CLEAR_COLOR_G = 255;
	constexpr int CLEAR_COLOR_B = 255;
	constexpr int CLEAR_COLOR_A = 255;
}
