#include "Game.h"
#include "HudRenderer.h"
#include "LevelSaver.h"
#include "SoundLoader.h"
#include <string>

Game::Game(SDL_Renderer* setRenderer, const LevelSettings& settings) :
    renderer(setRenderer),
    placementModeCurrent(settings.creativeMode ? PlacementMode::wall :
        (settings.allowWallEdit ? PlacementMode::wall : PlacementMode::turret)),
    level(setRenderer, GameConfig::TILE_COUNT_X, GameConfig::TILE_COUNT_Y),
    levelSettings(settings),
    spawnTimer(settings.spawnIntervalS),
    roundTimer(settings.roundDelayS),
    currency(settings.startingCurrency, settings.currencyPerKill, settings.turretCost) {

    level.applySettings(settings);
    mix_ChunkSpawnUnit = SoundLoader::loadSound("Spawn Unit.ogg");
}

Game::~Game() {
    SoundLoader::deallocateSounds();
}

GameRunResult Game::run() {
    auto time1 = std::chrono::system_clock::now();
    const float dT = GameConfig::FRAME_DT;
    bool running = true;
    GameRunResult exitResult = GameRunResult::menu;

    while (running) {
        auto time2 = std::chrono::system_clock::now();
        std::chrono::duration<float> timeDelta = time2 - time1;
        float timeDeltaFloat = timeDelta.count();

        if (timeDeltaFloat >= dT) {
            time1 = time2;
            processEvents(running, exitResult);
            if (running) {
                update(renderer, dT);
                draw(renderer);
            }
        }
    }

    if (matchState == MatchState::won || matchState == MatchState::lost)
        return GameRunResult::menu;

    return exitResult;
}

void Game::processEvents(bool& running, GameRunResult& exitResult) {
    if (matchState != MatchState::playing) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                exitResult = GameRunResult::quit;
                return;
            }
            else if (event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN) {
                running = false;
                exitResult = GameRunResult::menu;
                return;
            }
        }
        return;
    }

    bool mouseDownThisFrame = false;

    //Process events.
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            running = false;
            exitResult = GameRunResult::quit;
            return;

        case SDL_MOUSEBUTTONDOWN:
            mouseDownThisFrame = (mouseDownStatus == 0);
            if (event.button.button == SDL_BUTTON_LEFT)
                mouseDownStatus = SDL_BUTTON_LEFT;
            else if (event.button.button == SDL_BUTTON_RIGHT)
                mouseDownStatus = SDL_BUTTON_RIGHT;

            if (levelSettings.creativeMode && event.button.button == SDL_BUTTON_LEFT)
                handleHudClick(event.button.x, event.button.y);
            break;
        case SDL_MOUSEBUTTONUP:
            mouseDownStatus = 0;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                running = false;
                exitResult = GameRunResult::menu;
                break;
            case SDL_SCANCODE_1:
                placementModeCurrent = PlacementMode::wall;
                break;
            case SDL_SCANCODE_2:
                placementModeCurrent = PlacementMode::turret;
                break;
            case SDL_SCANCODE_3:
                if (levelSettings.creativeMode)
                    placementModeCurrent = PlacementMode::spawner;
                break;
            case SDL_SCANCODE_4:
                if (levelSettings.creativeMode)
                    placementModeCurrent = PlacementMode::target;
                break;
            case SDL_SCANCODE_S:
                if (levelSettings.creativeMode)
                    saveAsLevel1();
                break;
            }
            break;
        }
    }

    //Process input from the mouse cursor.
    int mouseX = 0, mouseY = 0;
    SDL_GetMouseState(&mouseX, &mouseY);

    //Convert from the window's coordinate system to the game's coordinate system.
    Vector2D posMouse((float)mouseX / GameConfig::TILE_SIZE, (float)mouseY / GameConfig::TILE_SIZE);

    if (levelSettings.creativeMode && HudRenderer::isSaveButtonAt(mouseX, mouseY))
        return;

    if (mouseDownStatus > 0) {
        switch (mouseDownStatus) {
        case SDL_BUTTON_LEFT:
            switch (placementModeCurrent) {
            case PlacementMode::wall:
                if ((levelSettings.allowWallEdit || levelSettings.creativeMode) &&
                    !level.isTargetTile((int)posMouse.x, (int)posMouse.y))
                    level.setTileWall((int)posMouse.x, (int)posMouse.y, true);
                break;
            case PlacementMode::turret:
                if (mouseDownThisFrame)
                    addTurret(posMouse);
                break;
            case PlacementMode::spawner:
                if (levelSettings.creativeMode && mouseDownThisFrame &&
                    !level.isTargetTile((int)posMouse.x, (int)posMouse.y))
                    level.setSpawner((int)posMouse.x, (int)posMouse.y, true);
                break;
            case PlacementMode::target:
                if (levelSettings.creativeMode && mouseDownThisFrame && !level.hasTarget())
                    level.setTargetPosition((int)posMouse.x, (int)posMouse.y);
                break;
            }
            break;


        case SDL_BUTTON_RIGHT:
            if (levelSettings.creativeMode) {
                const int tileX = (int)posMouse.x;
                const int tileY = (int)posMouse.y;
                if (level.isTargetTile(tileX, tileY))
                    level.clearTarget();
                else {
                    level.setSpawner(tileX, tileY, false);
                    level.setTileWall(tileX, tileY, false);
                }
            }
            else if (levelSettings.allowWallEdit) {
                level.setTileWall((int)posMouse.x, (int)posMouse.y, false);
            }
            removeTurretsAtMousePosition(posMouse);
            break;
        }
    }
}

void Game::handleHudClick(int mouseX, int mouseY) {
    if (HudRenderer::isSaveButtonAt(mouseX, mouseY))
        saveAsLevel1();
}

void Game::saveAsLevel1() {
    if (!level.hasTarget()) {
        saveMessage = "Place target first (4)";
        saveMessageTimerS = 2.0f;
        return;
    }
    if (!level.hasSpawners()) {
        saveMessage = "Place spawner first (3)";
        saveMessageTimerS = 2.0f;
        return;
    }

    std::vector<std::pair<int, int>> walls;
    std::vector<std::pair<int, int>> spawners;
    int targetX = 0;
    int targetY = 0;
    level.collectLayout(walls, spawners, targetX, targetY);

    if (LevelSaver::saveLayout("Data/Levels/level1.txt", targetX, targetY, spawners, walls)) {
        saveMessage = "Level 1 saved";
        saveMessageTimerS = 2.0f;
    }
    else {
        saveMessage = "Save failed";
        saveMessageTimerS = 2.0f;
    }
}

void Game::update(SDL_Renderer* renderer, float dT) {
    if (saveMessageTimerS > 0.0f)
        saveMessageTimerS -= dT;

    //Update the units.
    updateUnits(dT);

    //Update the turrets.
    for (auto& turretSelected : listTurrets)
        turretSelected.update(renderer, dT, listUnits, listProjectiles);

    //Update the projectiles.
    updateProjectiles(dT);

    updateSpawnUnitsIfRequired(renderer, dT);
    updateMatchState(dT);
}

void Game::updateUnits(float dT) {
    //Loop through the list of units and update all of them.
    auto it = listUnits.begin();
    while (it != listUnits.end()) {
        bool increment = true;

        if ((*it) != nullptr) {
            (*it)->update(dT, level, listUnits);

            //Check if the unit is still alive.  If not then erase it and don't increment the iterator.
            if ((*it)->isAlive() == false) {
                onUnitRemoved(*it);
                it = listUnits.erase(it);
                increment = false;
            }
        }

        if (increment)
            it++;
    }
}

void Game::onUnitRemoved(const std::shared_ptr<Unit>& unit) {
    if (unit == nullptr)
        return;

    switch (unit->getDeathReason()) {
    case UnitDeathReason::killed:
        currency.onUnitKilled();
        break;
    case UnitDeathReason::leaked:
        escaped++;
        break;
    default:
        break;
    }

    if (!levelSettings.creativeMode && escaped >= levelSettings.maxLeaks)
        matchState = MatchState::lost;
}

void Game::updateProjectiles(float dT) {
    //Loop through the list of projectiles and update all of them.
    auto it = listProjectiles.begin();
    while (it != listProjectiles.end()) {
        (*it).update(dT, listUnits);

        //Check if the projectile has collided or not, erase it if needed, and update the iterator.
        if ((*it).getCollisionOccurred())
            it = listProjectiles.erase(it);
        else
            it++;
    }
}

void Game::updateSpawnUnitsIfRequired(SDL_Renderer* renderer, float dT) {
    if (matchState != MatchState::playing || levelSettings.creativeMode)
        return;

    if (!level.hasSpawners() || !level.hasTarget())
        return;

    spawnTimer.countDown(dT);

    //Add a unit if needed.
    if (spawnUnitCount > 0 && spawnTimer.timeSIsZero()) {
        addUnit(level.getRandomEnemySpawnerLocation());

        //Play the spawn unit sound.
        if (mix_ChunkSpawnUnit != nullptr)
            Mix_PlayChannel(-1, mix_ChunkSpawnUnit, 0);

        spawnUnitCount--;
        spawnTimer.resetToMax();
    }

    if (currentRound >= levelSettings.maxRounds)
        return;

    //Check if the round needs to start.
    if (listUnits.empty() && spawnUnitCount == 0) {
        roundTimer.countDown(dT);
        if (roundTimer.timeSIsZero()) {
            currentRound++;
            spawnUnitCount = levelSettings.unitsPerRound +
                (currentRound - 1) * GameConfig::UNITS_PER_ROUND_GROWTH;
            roundTimer.resetToMax();
        }
    }
}

void Game::updateMatchState(float dT) {
    if (matchState != MatchState::playing || levelSettings.creativeMode)
        return;

    if (currentRound >= levelSettings.maxRounds && listUnits.empty() &&
        spawnUnitCount == 0 && escaped < levelSettings.maxLeaks)
        matchState = MatchState::won;
}

void Game::draw(SDL_Renderer* renderer) {
    //Draw.
    SDL_SetRenderDrawColor(renderer,
        GameConfig::CLEAR_COLOR.r,
        GameConfig::CLEAR_COLOR.g,
        GameConfig::CLEAR_COLOR.b,
        GameConfig::CLEAR_COLOR.a);
    //Clear the screen.
    SDL_RenderClear(renderer);

    SDL_Rect playfield = { 0, 0, GameConfig::WINDOW_WIDTH, GameConfig::WINDOW_HEIGHT };
    SDL_RenderSetViewport(renderer, &playfield);

    //Draw the level.
    level.draw(renderer, GameConfig::TILE_SIZE);

    //Draw the enemy units.
    for (auto& unitSelected : listUnits)
        if (unitSelected != nullptr)
            unitSelected->draw(renderer, GameConfig::TILE_SIZE);

    //Draw turrets
    for (auto& turretSelected : listTurrets)
        turretSelected.draw(renderer, GameConfig::TILE_SIZE);

    //Draw projectiles
    for (auto& projectSelected : listProjectiles)
        projectSelected.draw(renderer, GameConfig::TILE_SIZE);

    SDL_RenderSetViewport(renderer, NULL);

    drawHud(renderer);

    //Send the image to the window.
    SDL_RenderPresent(renderer);
}

void Game::drawHud(SDL_Renderer* renderer) {
    if (levelSettings.creativeMode) {
        const char* active = "Wall";
        switch (placementModeCurrent) {
        case PlacementMode::turret:
            active = "Turret";
            break;
        case PlacementMode::spawner:
            active = "Spawner";
            break;
        case PlacementMode::target:
            active = "Target";
            break;
        default:
            break;
        }

        HudRenderer::drawHudBox(renderer, 10, 10, 420, 48, UiColor{255, 196, 64, 255});
        HudRenderer::drawText(renderer, 22, 18, "1 Wall", UiColor{150, 154, 168, 255}, 16);
        HudRenderer::drawText(renderer, 90, 18, "2 Turret", UiColor{150, 154, 168, 255}, 16);
        HudRenderer::drawText(renderer, 178, 18, "3 Spawner", UiColor{150, 154, 168, 255}, 16);
        HudRenderer::drawText(renderer, 282, 18, "4 Target", UiColor{150, 154, 168, 255}, 16);
        HudRenderer::drawText(renderer, 22, 38, std::string("Mode: ") + active,
            UiColor{255, 196, 64, 255}, 16);

        const int saveX = GameConfig::WINDOW_WIDTH - 10 - 148;
        HudRenderer::drawHudBox(renderer, saveX, 10, 148, 34, UiColor{255, 196, 64, 255});
        HudRenderer::drawTextCentered(renderer, saveX + 74, 19, "Save Level 1",
            UiColor{235, 235, 240, 255}, 16);

        if (saveMessageTimerS > 0.0f && !saveMessage.empty()) {
            const int msgW = HudRenderer::measureTextWidth(saveMessage, 16) + 24;
            const int bx = (GameConfig::WINDOW_WIDTH - msgW) / 2;
            const int by = 66;
            HudRenderer::drawHudBox(renderer, bx, by, msgW, 34, UiColor{255, 196, 64, 255});
            HudRenderer::drawTextCentered(renderer, GameConfig::WINDOW_WIDTH / 2, by + 9,
                saveMessage, UiColor{96, 220, 120, 255}, 16);
        }
        return;
    }

    if (matchState == MatchState::won) {
        const int boxW = HudRenderer::measureTextWidth("You Win!  Press any key", 24) + 48;
        const int bx = (GameConfig::WINDOW_WIDTH - boxW) / 2;
        HudRenderer::drawHudBox(renderer, bx, 10, boxW, 40, UiColor{96, 220, 120, 255});
        HudRenderer::drawTextCentered(renderer, GameConfig::WINDOW_WIDTH / 2, 20,
            "You Win!  Press any key", UiColor{96, 220, 120, 255}, 24);
        return;
    }
    if (matchState == MatchState::lost) {
        const int boxW = HudRenderer::measureTextWidth("You Lose!  Press any key", 24) + 48;
        const int bx = (GameConfig::WINDOW_WIDTH - boxW) / 2;
        HudRenderer::drawHudBox(renderer, bx, 10, boxW, 40, UiColor{235, 55, 55, 255});
        HudRenderer::drawTextCentered(renderer, GameConfig::WINDOW_WIDTH / 2, 20,
            "You Lose!  Press any key", UiColor{235, 55, 55, 255}, 24);
        return;
    }

    const std::string escapedText =
        std::to_string(escaped) + "/" + std::to_string(levelSettings.maxLeaks);
    const std::string coinsText = std::to_string(currency.getBalance());

    int endX = 22 + 52;
    endX += HudRenderer::measureTextWidth(coinsText, 16);
    endX += 20 + HudRenderer::measureTextWidth("Escaped", 16) + 10;
    endX += HudRenderer::measureTextWidth(escapedText, 16);

    HudRenderer::drawHudBox(renderer, 10, 10, endX - 10 + 12, 34, UiColor{255, 196, 64, 255});

    int x = 22;
    HudRenderer::drawText(renderer, x, 20, "Coins", UiColor{150, 154, 168, 255}, 16);
    x += 52;
    HudRenderer::drawText(renderer, x, 18, coinsText, UiColor{255, 196, 64, 255}, 16);
    x += HudRenderer::measureTextWidth(coinsText, 16) + 20;
    HudRenderer::drawText(renderer, x, 20, "Escaped", UiColor{235, 55, 55, 255}, 16);
    x += HudRenderer::measureTextWidth("Escaped", 16) + 10;
    HudRenderer::drawText(renderer, x, 18, escapedText, UiColor{235, 55, 55, 255}, 16);
}

void Game::addUnit(Vector2D pos) {
    listUnits.push_back(std::make_shared<Unit>(renderer, pos));
}

void Game::addTurret(Vector2D posMouse) {
    int tileX = (int)posMouse.x;
    int tileY = (int)posMouse.y;

    if (!levelSettings.creativeMode && levelSettings.turretsOnWallsOnly &&
        !level.isTileWall(tileX, tileY))
        return;

    if (level.isTileSpawner(tileX, tileY) || level.isTargetTile(tileX, tileY))
        return;

    for (const auto& turretSelected : listTurrets) {
        if (turretSelected.checkIfOnTile(tileX, tileY))
            return;
    }

    if (!levelSettings.creativeMode && !currency.trySpendOnTurret())
        return;

    Vector2D pos(tileX + GameConfig::TILE_CENTER_OFFSET, tileY + GameConfig::TILE_CENTER_OFFSET);
    listTurrets.push_back(Turret(renderer, pos));
}

void Game::removeTurretsAtMousePosition(Vector2D posMouse) {
    for (auto it = listTurrets.begin(); it != listTurrets.end();) {
        if ((*it).checkIfOnTile((int)posMouse.x, (int)posMouse.y))
            it = listTurrets.erase(it);
        else
            it++;
    }
}
