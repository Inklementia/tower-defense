#pragma once
#include <string>
#include <utility>
#include <vector>

class LevelSaver {
public:
  static bool saveLayout(const std::string &filename, int targetX, int targetY,
                         const std::vector<std::pair<int, int>> &spawners,
                         const std::vector<std::pair<int, int>> &walls);
};
