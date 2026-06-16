#pragma once
#include "SDL.h"
#include <vector>
#include <chrono>
#include <memory>
#include "GameConfig.h"
#include "Timer.h"
#include "Unit.h"
#include "Turret.h"
#include "Level.h"

class Game {
private:
	enum class PlacementMode {
		wall,
		unit,
		turret,
	} placementModeCurrent;

public:
	Game(SDL_Window* window, SDL_Renderer* renderer);
	~Game();

private:
	void processEvents(SDL_Renderer* renderer, bool& running);
	void update(SDL_Renderer* renderer, float dT);
	void updateUnits(float dT);
	void updateSpawnUnitsIfRequired(SDL_Renderer* renderer, float dT);
	void draw(SDL_Renderer* renderer);
	void addUnit(SDL_Renderer* renderer, Vector2D posMouse);
	void addTurret(SDL_Renderer* renderer, Vector2D posMouse);
	void removeTurretsAtMousePosition(Vector2D posMouse);

	int mouseDownStatus = 0;

	Level level;
	
	std::vector<std::shared_ptr<Unit>> listUnits;
	std::vector<Turret> listTurrets;

	SDL_Texture* textureOverlay = nullptr;
	bool overlayVisible = true;

	Timer spawnTimer, roundTimer;
	int spawnUnitCount = 0;
};