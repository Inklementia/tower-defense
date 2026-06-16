#pragma once
#include <memory>
#include <vector>
#include "SDL.h"
#include "MathUtils.h"
#include "Vector2D.h"
#include "TextureLoader.h"

class Unit;

class Turret {
public:
	Turret(SDL_Renderer* renderer, Vector2D setPos);
	void update(float dT, std::vector<std::shared_ptr<Unit>>& listUnits);
	void draw(SDL_Renderer* renderer, int tileSize);
	bool checkIfOnTile(int x, int y);

private:
	std::weak_ptr<Unit> findEnemyUnit(std::vector<std::shared_ptr<Unit>>& listUnits);
	void drawTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* textureSelected,
		int offset, int tileSize);

	Vector2D pos;
	float angle;
	static const float speedAngular, weaponRange;

	std::weak_ptr<Unit> unitTarget;

	SDL_Texture* textureMain = nullptr,
	*textureShadow = nullptr;
};
