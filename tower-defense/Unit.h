#pragma once
#include <memory>
#include <vector>
#include "SDL.h"
#include "Vector2D.h"
#include "Level.h"
#include "TextureLoader.h"

class Unit {
public:
	Unit(SDL_Renderer* renderer, Vector2D setPos);

	void update(float dT, Level& level, std::vector<std::shared_ptr<Unit>>& listUnits);
	void draw(SDL_Renderer* renderer, int tileSize);
	bool checkOverlap(Vector2D posOther, float sizeOther);
	bool getIsAlive();
	Vector2D getPos();

private:
	Vector2D pos;
	static const float speed;
	static const float size;

	SDL_Texture* texture = nullptr;

	bool isAlive = true;
};