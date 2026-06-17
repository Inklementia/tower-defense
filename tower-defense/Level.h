#pragma once
#include "GameConfig.h"
#include "LevelSettings.h"
#include "SDL.h"
#include "TextureLoader.h"
#include "Vector2D.h"
#include <queue>
#include <vector>

class Level {
private:
  enum class TileType : char { empty, wall, enemySpawner };

  static const unsigned char flowDistanceMax = GameConfig::FLOW_DISTANCE_MAX;

  struct Tile {
    TileType type = TileType::empty;
    int flowDirectionX = 0;
    int flowDirectionY = 0;
    unsigned char flowDistance = flowDistanceMax;
  };

public:
  Level(SDL_Renderer *renderer, int setTileCountX, int setTileCountY);
  void applySettings(const LevelSettings &settings);
  void draw(SDL_Renderer *renderer, int tileSize);

  Vector2D getRandomEnemySpawnerLocation();
  bool isTileWall(int x, int y);
  bool isTileSpawner(int x, int y);
  bool hasTarget() const;
  bool hasSpawners() const;
  bool isTargetTile(int x, int y) const;
  void setTileWall(int x, int y, bool setWall);
  void setSpawner(int x, int y, bool add);
  void setTargetPosition(int x, int y);
  void clearTarget();
  void collectLayout(std::vector<std::pair<int, int>> &wallsOut,
                     std::vector<std::pair<int, int>> &spawnersOut,
                     int &targetXOut, int &targetYOut) const;
  Vector2D getTargetPos();
  Vector2D getFlowNormal(int x, int y);

private:
  TileType getTileType(int x, int y);
  void setTileType(int x, int y, TileType tileType);
  void setTileTypeWithoutRecalc(int x, int y, TileType tileType);
  void drawTile(SDL_Renderer *renderer, int x, int y, int tileSize);
  void calculateFlowField();
  void calculateDistances();
  void calculateFlowDirections();

  std::vector<Tile> listTiles;
  const int tileCountX, tileCountY;

  int targetX = 0;
  int targetY = 0;
  bool targetPlaced = false;
  bool showFlowArrows = false;

  SDL_Texture *textureTileWall = nullptr, *textureTileTarget = nullptr,
              *textureTileEnemySpawner = nullptr, *textureTileEmpty = nullptr,
              *textureTileArrowUp = nullptr, *textureTileArrowUpRight = nullptr,
              *textureTileArrowRight = nullptr,
              *textureTileArrowDownRight = nullptr,
              *textureTileArrowDown = nullptr,
              *textureTileArrowDownLeft = nullptr,
              *textureTileArrowLeft = nullptr,
              *textureTileArrowUpLeft = nullptr;
};