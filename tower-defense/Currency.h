#pragma once

class Currency {
public:
	Currency(int startingAmount, int rewardPerKill, int turretCost);

	int getBalance() const;
	int getTurretCost() const;

	void onUnitKilled();
	bool trySpendOnTurret();

private:
	int balance = 0;
	int rewardPerKill = 0;
	int turretCost = 0;
};
