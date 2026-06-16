#pragma once

namespace GameConfig {
	struct Color {
		int r, g, b, a;

		constexpr Color(int setR, int setG, int setB, int setA) :
			r(setR), g(setG), b(setB), a(setA) {}
	};

	// Window	
	constexpr int WINDOW_WIDTH = 960;
	constexpr int WINDOW_HEIGHT = 576;

	// Grid
	constexpr int TILE_SIZE = 64;
	constexpr int TILE_COUNT_X = WINDOW_WIDTH / TILE_SIZE;
	constexpr int TILE_COUNT_Y = WINDOW_HEIGHT / TILE_SIZE;
	constexpr float TILE_CENTER_OFFSET = 0.5f;

	// Math	
	constexpr float DEGREES_IN_HALF_CIRCLE = 180.0f;
	constexpr float PI = 3.14159265359f;

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
	constexpr float UNIT_COLLISION_ANGLE = PI / 4.0f;
	constexpr int UNIT_HEALTH_MAX = 2;
	constexpr float UNIT_HURT_FLASH_S = 0.25f;
	constexpr Color UNIT_TEXTURE_NORMAL{ 255, 255, 255, 255 };
	constexpr Color UNIT_TEXTURE_HURT{ 255, 0, 0, 255 };

	// Turrets
	constexpr float TURRET_ROTATION_SPEED_DEG = 180.0f;
	constexpr float TURRET_WEAPON_RANGE = 5.0f;
	constexpr float TURRET_WEAPON_COOLDOWN_S = 1.0f;
	constexpr int TURRET_SHADOW_OFFSET = 5;

	// Projectiles
	constexpr float PROJECTILE_SPEED = 10.0f;
	constexpr float PROJECTILE_SIZE = 0.2f;
	constexpr float PROJECTILE_MAX_DISTANCE = 20.0f;
	constexpr int PROJECTILE_DAMAGE = 1;

	// Audio
	constexpr int AUDIO_FREQUENCY = 44100;
	constexpr int AUDIO_CHANNELS = 2;
	constexpr int AUDIO_CHUNK_SIZE = 1024;
	constexpr int AUDIO_MIXER_CHANNELS = 32;

	// Level / flow field
	constexpr unsigned char FLOW_DISTANCE_MAX = 255;

	// UI
	constexpr int OVERLAY_X = 40;
	constexpr int OVERLAY_Y = 40;

	// Colors
	constexpr Color CLEAR_COLOR{ 255, 255, 255, 255 };
	constexpr Color LEVEL_CHECKER_LIGHT{ 240, 240, 240, 255 };
	constexpr Color LEVEL_CHECKER_DARK{ 225, 225, 225, 255 };

}
