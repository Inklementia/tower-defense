#pragma once
#include "Currency.h"
#include "GameConfig.h"
#include "Level.h"
#include "LevelSettings.h"
#include "Projectile.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include "Timer.h"
#include "Turret.h"
#include "Unit.h"
#include <chrono>
#include <memory>
#include <vector>

enum class GameRunResult { menu, quit };

class Game {
private:
	enum class PlacementMode {
		wall,
		turret,
		spawner,
		target,
	} placementModeCurrent;

	enum class MatchState { playing, won, lost };

public:
	Game(SDL_Renderer *renderer, const LevelSettings &settings);
	~Game();

	GameRunResult run();

private:
	void processEvents(bool &running, GameRunResult &exitResult);
	void update(SDL_Renderer *renderer, float dT);
	void updateUnits(float dT);
	void updateProjectiles(float dT);
	void updateSpawnUnitsIfRequired(SDL_Renderer *renderer, float dT);
	void updateMatchState(float dT);
	void draw(SDL_Renderer *renderer);
	void drawHud(SDL_Renderer *renderer);
	void handleHudClick(int mouseX, int mouseY);
	void saveAsLevel1();
	void addUnit(Vector2D pos);
	void addTurret(Vector2D posMouse);
	void removeTurretsAtMousePosition(Vector2D posMouse);
	void onUnitRemoved(const std::shared_ptr<Unit> &unit);

	int mouseDownStatus = 0;

	SDL_Renderer *renderer = nullptr;

	Level level;
	LevelSettings levelSettings;

	std::vector<std::shared_ptr<Unit>> listUnits;
	std::vector<Turret> listTurrets;
	std::vector<Projectile> listProjectiles;

	Timer spawnTimer;
	Timer roundTimer;
	int spawnUnitCount = 0;
	int currentRound = 0;

	int escaped = 0;
	Currency currency;
	MatchState matchState = MatchState::playing;

	std::string saveMessage;
	float saveMessageTimerS = 0.0f;

	Mix_Chunk *mix_ChunkSpawnUnit = nullptr;
};