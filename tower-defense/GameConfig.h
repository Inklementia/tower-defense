#pragma once

namespace GameConfig {
struct Color {
  int r, g, b, a;

  constexpr Color(int setR, int setG, int setB, int setA)
      : r(setR), g(setG), b(setB), a(setA) {}
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
constexpr float FRAME_DT = 1.0f / 60.0f;

// Units
constexpr float UNIT_SPEED = 1.5f;
constexpr float UNIT_SIZE = 0.4f;
constexpr float UNIT_WALL_SPACING = 0.35f;
constexpr float UNIT_TARGET_REACH_DISTANCE = 0.5f;
constexpr float UNIT_OVERLAP_MIN_DISTANCE = 0.01f;
constexpr float UNIT_COLLISION_ANGLE = PI / 4.0f;
constexpr int UNIT_HEALTH_MAX = 2;
constexpr float UNIT_HURT_FLASH_S = 0.125f;
constexpr int UNIT_HURT_FLASH_COUNT = 2;
constexpr int UNIT_HEALTH_BAR_WIDTH = 24;
constexpr int UNIT_HEALTH_BAR_HEIGHT = 4;
constexpr int UNIT_HEALTH_BAR_OFFSET_Y = 2;
constexpr Color UNIT_TEXTURE_NORMAL{255, 255, 255, 255};
constexpr Color UNIT_TEXTURE_HURT{255, 255, 255, 255};
constexpr Color UNIT_TEXTURE_HURT_DIM{160, 160, 160, 255};
constexpr Color UNIT_HEALTH_BAR_BG{0, 0, 0, 200};
constexpr Color UNIT_HEALTH_BAR_FILL{60, 200, 80, 255};

// Turrets
constexpr float TURRET_ROTATION_SPEED_DEG = 180.0f;
constexpr float TURRET_WEAPON_RANGE = 4.0f;
constexpr float TURRET_WEAPON_COOLDOWN_S = 1.0f;
constexpr int TURRET_SHADOW_OFFSET = 5;
constexpr Color TURRET_RANGE_OUTLINE{120, 200, 255, 180};
constexpr int TURRET_RANGE_LINE_WIDTH = 3;

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

// Playfield colors
constexpr Color CLEAR_COLOR{225, 225, 225, 255};
constexpr Color LEVEL_CHECKER_LIGHT{240, 240, 240, 255};
constexpr Color LEVEL_CHECKER_DARK{225, 225, 225, 255};

} // namespace GameConfig
