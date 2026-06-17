#include "Currency.h"

Currency::Currency(int startingAmount, int rewardPerKill, int turretCost)
    : balance(startingAmount < 0 ? 0 : startingAmount),
      rewardPerKill(rewardPerKill), turretCost(turretCost) {}

int Currency::getBalance() const { return balance; }

int Currency::getTurretCost() const { return turretCost; }

void Currency::onUnitKilled() { balance += rewardPerKill; }

bool Currency::trySpendOnTurret() {
  if (turretCost <= 0)
    return true;

  if (balance < turretCost)
    return false;

  balance -= turretCost;
  if (balance < 0)
    balance = 0;
  return true;
}
