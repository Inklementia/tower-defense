#include "LevelSaver.h"
#define WIN32_LEAN_AND_MEAN
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>


static std::string trim(const std::string &value) {
  size_t start = value.find_first_not_of(" \t\r\n");
  if (start == std::string::npos)
    return "";
  size_t end = value.find_last_not_of(" \t\r\n");
  return value.substr(start, end - start + 1);
}

static bool isLayoutLine(const std::string &line) {
  if (line.rfind("wall=", 0) == 0)
    return true;
  if (line.rfind("spawner=", 0) == 0)
    return true;
  if (line.rfind("target=", 0) == 0)
    return true;
  return false;
}

static std::string getExecutableDirectory() {
  char path[MAX_PATH];
  DWORD len = GetModuleFileNameA(NULL, path, MAX_PATH);
  if (len == 0 || len >= MAX_PATH)
    return "";

  std::string base(path, len);
  size_t pos = base.find_last_of("\\/");
  if (pos == std::string::npos)
    return "";

  return base.substr(0, pos + 1);
}

static std::string normalizePath(const std::string &path) {
  char fullPath[MAX_PATH];
  DWORD len = GetFullPathNameA(path.c_str(), MAX_PATH, fullPath, NULL);
  if (len == 0 || len >= MAX_PATH)
    return path;

  return std::string(fullPath);
}

static std::string getSourceDataPath(const std::string &relativePath) {
  std::string exeDir = getExecutableDirectory();
  if (exeDir.empty())
    return "";

  std::string candidate = exeDir + "..\\..\\tower-defense\\" + relativePath;
  std::string fullPath = normalizePath(candidate);

  size_t pos = fullPath.find_last_of("\\/");
  if (pos == std::string::npos)
    return "";

  std::string dir = fullPath.substr(0, pos);
  DWORD attrs = GetFileAttributesA(dir.c_str());
  if (attrs == INVALID_FILE_ATTRIBUTES || !(attrs & FILE_ATTRIBUTE_DIRECTORY))
    return "";

  return fullPath;
}

static std::vector<std::string> readPreservedLines(const std::string &path) {
  std::vector<std::string> preservedLines;
  std::ifstream input(path);
  if (!input.is_open())
    return preservedLines;

  std::string line;
  while (std::getline(input, line)) {
    line = trim(line);
    if (line.empty() || line[0] == '#')
      continue;
    if (!isLayoutLine(line))
      preservedLines.push_back(line);
  }

  return preservedLines;
}

static bool writeLayoutFile(const std::string &path,
                            const std::vector<std::string> &preservedLines,
                            int targetX, int targetY,
                            const std::vector<std::pair<int, int>> &spawners,
                            const std::vector<std::pair<int, int>> &walls) {

  std::ofstream output(path);
  if (!output.is_open())
    return false;

  for (const auto &line : preservedLines)
    output << line << '\n';

  output << "target=" << targetX << ',' << targetY << '\n';
  for (const auto &spawner : spawners)
    output << "spawner=" << spawner.first << ',' << spawner.second << '\n';
  for (const auto &wall : walls)
    output << "wall=" << wall.first << ',' << wall.second << '\n';

  return true;
}

bool LevelSaver::saveLayout(const std::string &filename, int targetX,
                            int targetY,
                            const std::vector<std::pair<int, int>> &spawners,
                            const std::vector<std::pair<int, int>> &walls) {

  const std::string sourcePath = getSourceDataPath(filename);

  std::vector<std::string> preservedLines = readPreservedLines(filename);
  if (preservedLines.empty() && !sourcePath.empty())
    preservedLines = readPreservedLines(sourcePath);

  if (!writeLayoutFile(filename, preservedLines, targetX, targetY, spawners,
                       walls)) {
    std::cout << "Error: Could not save level file " << filename << std::endl;
    return false;
  }

  if (!sourcePath.empty() && sourcePath != normalizePath(filename)) {
    if (!writeLayoutFile(sourcePath, preservedLines, targetX, targetY, spawners,
                         walls)) {
      std::cout << "Warning: Could not save source level file " << sourcePath
                << std::endl;
    }
  }

  return true;
}
