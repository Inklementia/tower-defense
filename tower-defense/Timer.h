#pragma once

class Timer {
public:
  Timer(float setTimeSMax, float setTimeSCurrent = 0.0f);

  void countUp(float dT);
  void countDown(float dT);
  void resetToZero();
  void resetToMax();
  bool timeSIsZero();
  bool timeSIsGreaterThanOrEqualTo(float timeSCheck);
  float getTimeSCurrent() const { return timeSCurrent; }
  float getTimeSMax() const { return timeSMax; }

private:
  float timeSMax;
  float timeSCurrent;
};