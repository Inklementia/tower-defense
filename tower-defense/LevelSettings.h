#pragma once
#include <string>
#include <utility>
#include <vector>

struct LevelSettings {
  int targetX = 0;
  int targetY = 0;
  std::vector<std::pair<int, int>> spawners;
  std::vector<std::pair<int, int>> walls;

  float spawnIntervalS = 0.25f;
  float roundDelayS = 5.0f;
  int unitsPerRound = 15;
  int maxRounds = 3;

  int startingCurrency = 1;
  int currencyPerKill = 1;
  int currencyPerLeak = -1;
  int maxLeaks = 10;
  int turretCost = 1;
  bool allowWallEdit = true;
  bool turretsOnWallsOnly = false;
  bool showFlowArrows = true;
  bool creativeMode = false;
};

class LevelLoader {
public:
  static LevelSettings load(const std::string &filename);
  static std::string getLevelPath(int levelIndex);
};
