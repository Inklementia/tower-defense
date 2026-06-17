#include "GameConfig.h"
#include "LevelSettings.h"
#include <fstream>
#include <iostream>

static std::string trim(const std::string &value) {
  size_t start = value.find_first_not_of(" \t\r\n");
  if (start == std::string::npos)
    return "";
  size_t end = value.find_last_not_of(" \t\r\n");
  return value.substr(start, end - start + 1);
}

static bool parseCoordinate(const std::string &value, int &x, int &y) {
  size_t comma = value.find(',');
  if (comma == std::string::npos)
    return false;
  x = std::stoi(value.substr(0, comma));
  y = std::stoi(value.substr(comma + 1));
  return true;
}

LevelSettings LevelLoader::load(const std::string &filename) {
  LevelSettings settings;
  settings.targetX = GameConfig::TILE_COUNT_X / 2;
  settings.targetY = GameConfig::TILE_COUNT_Y / 2;

  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cout << "Warning: Could not load level file " << filename << std::endl;
    return settings;
  }

  std::string line;
  while (std::getline(file, line)) {
    line = trim(line);
    if (line.empty() || line[0] == '#')
      continue;

    size_t eq = line.find('=');
    if (eq == std::string::npos)
      continue;

    std::string key = trim(line.substr(0, eq));
    std::string value = trim(line.substr(eq + 1));

    if (key == "target") {
      parseCoordinate(value, settings.targetX, settings.targetY);
    } else if (key == "spawner") {
      int x = 0, y = 0;
      if (parseCoordinate(value, x, y))
        settings.spawners.push_back({x, y});
    } else if (key == "wall") {
      int x = 0, y = 0;
      if (parseCoordinate(value, x, y))
        settings.walls.push_back({x, y});
    } else if (key == "spawn_interval") {
      settings.spawnIntervalS = std::stof(value);
    } else if (key == "round_delay") {
      settings.roundDelayS = std::stof(value);
    } else if (key == "units_per_round") {
      settings.unitsPerRound = std::stoi(value);
    } else if (key == "max_rounds") {
      settings.maxRounds = std::stoi(value);
    } else if (key == "unit_health_multiplier") {
      settings.unitHealthMultiplier = std::stof(value);
    } else if (key == "units_per_round_multiplier") {
      settings.unitsPerRoundMultiplier = std::stof(value);
    } else if (key == "starting_currency") {
      settings.startingCurrency = std::stoi(value);
    } else if (key == "currency_per_kill") {
      settings.currencyPerKill = std::stof(value);
    } else if (key == "currency_per_leak") {
      settings.currencyPerLeak = std::stoi(value);
    } else if (key == "max_leaks") {
      settings.maxLeaks = std::stoi(value);
    } else if (key == "turret_cost") {
      settings.turretCost = std::stoi(value);
    } else if (key == "allow_wall_edit") {
      settings.allowWallEdit =
          (value == "1" || value == "true" || value == "yes");
    } else if (key == "turrets_on_walls_only") {
      settings.turretsOnWallsOnly =
          (value == "1" || value == "true" || value == "yes");
    } else if (key == "show_flow_arrows") {
      settings.showFlowArrows =
          (value == "1" || value == "true" || value == "yes");
    } else if (key == "creative_mode") {
      settings.creativeMode =
          (value == "1" || value == "true" || value == "yes");
    }
  }

  return settings;
}

std::string LevelLoader::getLevelPath(int levelIndex) {
  switch (levelIndex) {
  case 1:
    return "Data/Levels/sandbox.txt";
  default:
    return "Data/Levels/level1.txt";
  }
}
