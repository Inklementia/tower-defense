#include "MathUtils.h"


const float MathUtils::PI = 3.14159265359f;

float MathUtils::angleRadToDeg(float angle) {
	return angle * 180.0f / PI;
}

float MathUtils::angleDegToRad(float angle) {
	return angle * PI / 180.0f;
}