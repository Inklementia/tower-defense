#pragma once
#include "GameConfig.h"
#include "Level.h"
#include "SDL.h"
#include "TextureLoader.h"
#include "Timer.h"
#include "Vector2D.h"
#include <memory>
#include <vector>


enum class UnitDeathReason { none, killed, leaked };

class Unit {
public:
  Unit(SDL_Renderer *renderer, Vector2D setPos);

  void update(float dT, Level &level,
              std::vector<std::shared_ptr<Unit>> &listUnits);
  void draw(SDL_Renderer *renderer, int tileSize);
  bool checkOverlap(Vector2D posOther, float sizeOther);
  bool isAlive();
  Vector2D getPos();
  void removeHealth(int damage);
  UnitDeathReason getDeathReason() const;

private:
  Vector2D pos;
  static const float speed;
  static const float size;

  SDL_Texture *texture = nullptr;

  Timer timerJustHurt;
  UnitDeathReason deathReason = UnitDeathReason::none;

  const int healthMax = GameConfig::UNIT_HEALTH_MAX;
  int healthCurrent = healthMax;
};