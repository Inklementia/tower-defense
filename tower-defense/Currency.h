#pragma once
#include <string>

class Currency {
public:
  Currency(float startingAmount, float rewardPerKill, int turretCost);

  float getBalance() const;
  std::string getBalanceText() const;
  int getTurretCost() const;

  void onUnitKilled();
  bool trySpendOnTurret();

private:
  float balance = 0.0f;
  float rewardPerKill = 0.0f;
  int turretCost = 0;
};
