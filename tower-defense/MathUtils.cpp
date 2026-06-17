#include "MathUtils.h"
#include "GameConfig.h"

const float MathUtils::PI = GameConfig::PI;

float MathUtils::angleRadToDeg(float angle) {
  return angle * GameConfig::DEGREES_IN_HALF_CIRCLE / PI;
}

float MathUtils::angleDegToRad(float angle) {
  return angle * PI / GameConfig::DEGREES_IN_HALF_CIRCLE;
}