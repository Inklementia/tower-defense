#include "Currency.h"
#include <cmath>
#include <cstdio>

Currency::Currency(float startingAmount, float rewardPerKill, int turretCost)
    : balance(startingAmount < 0.0f ? 0.0f : startingAmount),
      rewardPerKill(rewardPerKill), turretCost(turretCost) {}

float Currency::getBalance() const { return balance; }

std::string Currency::getBalanceText() const {
  if (balance == std::floor(balance))
    return std::to_string(static_cast<int>(balance));

  char buf[16];
  std::snprintf(buf, sizeof(buf), "%.1f", balance);
  return buf;
}

int Currency::getTurretCost() const { return turretCost; }

void Currency::onUnitKilled() { balance += rewardPerKill; }

bool Currency::trySpendOnTurret() {
  if (turretCost <= 0)
    return true;

  if (balance < static_cast<float>(turretCost))
    return false;

  balance -= static_cast<float>(turretCost);
  if (balance < 0.0f)
    balance = 0.0f;
  return true;
}
