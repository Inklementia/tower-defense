#include "Level.h"
#include "LevelSettings.h"

Level::Level(SDL_Renderer* renderer, int setTileCountX, int setTileCountY) :
    tileCountX(setTileCountX), tileCountY(setTileCountY),
    targetX(setTileCountX / 2), targetY(setTileCountY / 2) {
    textureTileWall = TextureLoader::loadTexture(renderer, "Tile Wall.bmp");
    textureTileTarget = TextureLoader::loadTexture(renderer, "Tile Target.bmp");
    textureTileEnemySpawner = TextureLoader::loadTexture(renderer, "Tile Enemy Spawner.bmp");

    textureTileEmpty = TextureLoader::loadTexture(renderer, "Tile Empty.bmp");
    textureTileArrowUp = TextureLoader::loadTexture(renderer, "Tile Arrow Up.bmp");
    textureTileArrowUpRight = TextureLoader::loadTexture(renderer, "Tile Arrow Up Right.bmp");
    textureTileArrowRight = TextureLoader::loadTexture(renderer, "Tile Arrow Right.bmp");
    textureTileArrowDownRight = TextureLoader::loadTexture(renderer, "Tile Arrow Down Right.bmp");
    textureTileArrowDown = TextureLoader::loadTexture(renderer, "Tile Arrow Down.bmp");
    textureTileArrowDownLeft = TextureLoader::loadTexture(renderer, "Tile Arrow Down Left.bmp");
    textureTileArrowLeft = TextureLoader::loadTexture(renderer, "Tile Arrow Left.bmp");
    textureTileArrowUpLeft = TextureLoader::loadTexture(renderer, "Tile Arrow Up Left.bmp");

    size_t listTilesSize = (size_t)tileCountX * tileCountY;
    listTiles.assign(listTilesSize, Tile{});
}

void Level::applySettings(const LevelSettings& settings) {
    targetX = settings.targetX;
    targetY = settings.targetY;
    targetPlaced = !settings.creativeMode;
    showFlowArrows = settings.showFlowArrows;

    for (auto& tileSelected : listTiles)
        tileSelected.type = TileType::empty;

    for (const auto& spawner : settings.spawners)
        setTileTypeWithoutRecalc(spawner.first, spawner.second, TileType::enemySpawner);

    for (const auto& wall : settings.walls)
        setTileTypeWithoutRecalc(wall.first, wall.second, TileType::wall);

    if (settings.spawners.empty() && !settings.creativeMode) {
        int xMax = tileCountX - 1;
        int yMax = tileCountY - 1;
        setTileTypeWithoutRecalc(0, 0, TileType::enemySpawner);
        setTileTypeWithoutRecalc(xMax, 0, TileType::enemySpawner);
        setTileTypeWithoutRecalc(0, yMax, TileType::enemySpawner);
        setTileTypeWithoutRecalc(xMax, yMax, TileType::enemySpawner);
    }

    if (targetPlaced)
        calculateFlowField();
}

void Level::draw(SDL_Renderer* renderer, int tileSize) {
    //Draw the tile's background color.
    for (int y = 0; y < tileCountY; y++) {
        for (int x = 0; x < tileCountX; x++) {
            if ((x + y) % 2 == 0)
                SDL_SetRenderDrawColor(renderer,
                    GameConfig::LEVEL_CHECKER_LIGHT.r,
                    GameConfig::LEVEL_CHECKER_LIGHT.g,
                    GameConfig::LEVEL_CHECKER_LIGHT.b,
                    GameConfig::LEVEL_CHECKER_LIGHT.a);
            else
                SDL_SetRenderDrawColor(renderer,
                    GameConfig::LEVEL_CHECKER_DARK.r,
                    GameConfig::LEVEL_CHECKER_DARK.g,
                    GameConfig::LEVEL_CHECKER_DARK.b,
                    GameConfig::LEVEL_CHECKER_DARK.a);

            SDL_Rect rect = { x * tileSize, y * tileSize, tileSize, tileSize };
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    if (showFlowArrows && targetPlaced) {
        for (int count = 0; count < (int)listTiles.size(); count++)
            drawTile(renderer, (count % tileCountX), (count / tileCountX), tileSize);
    }

    //Draw the wall tiles.
    if (textureTileWall != nullptr) {
        int w = 0, h = 0;
        SDL_QueryTexture(textureTileWall, NULL, NULL, &w, &h);
        for (int y = 0; y < tileCountY; y++) {
            for (int x = 0; x < tileCountX; x++) {
                if (isTileWall(x, y)) {
                    SDL_Rect rect = {
                        x * tileSize + tileSize / 2 - w / 2,
                        y * tileSize + tileSize / 2 - h / 2,
                        w,
                        h };
                    SDL_RenderCopy(renderer, textureTileWall, NULL, &rect);
                }
            }
        }
    }

    //Draw the enemy spawner tiles.
    if (textureTileEnemySpawner != nullptr) {
        for (int y = 0; y < tileCountY; y++) {
            for (int x = 0; x < tileCountX; x++) {
                if (getTileType(x, y) == TileType::enemySpawner) {
                    SDL_Rect rect = { x * tileSize, y * tileSize, tileSize, tileSize };
                    SDL_RenderCopy(renderer, textureTileEnemySpawner, NULL, &rect);
                }
            }
        }
    }

    //Draw the target tile.
    if (textureTileTarget != nullptr && targetPlaced) {
        SDL_Rect rect = { targetX * tileSize, targetY * tileSize, tileSize, tileSize };
        SDL_RenderCopy(renderer, textureTileTarget, NULL, &rect);
    }
}

void Level::drawTile(SDL_Renderer* renderer, int x, int y, int tileSize) {
    //Set the default texture image to be empty.
    SDL_Texture* textureSelected = textureTileEmpty;

    //Ensure that the input tile exists.
    int index = x + y * tileCountX;
    if (index > -1 && index < listTiles.size() &&
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY) {
        Tile& tileSelected = listTiles[index];

        //Select the correct tile texture based on the flow direction.
        if (tileSelected.flowDirectionX == 0 && tileSelected.flowDirectionY == -1)
            textureSelected = textureTileArrowUp;
        else if (tileSelected.flowDirectionX == 1 && tileSelected.flowDirectionY == -1)
            textureSelected = textureTileArrowUpRight;
        else if (tileSelected.flowDirectionX == 1 && tileSelected.flowDirectionY == 0)
            textureSelected = textureTileArrowRight;
        else if (tileSelected.flowDirectionX == 1 && tileSelected.flowDirectionY == 1)
            textureSelected = textureTileArrowDownRight;
        else if (tileSelected.flowDirectionX == 0 && tileSelected.flowDirectionY == 1)
            textureSelected = textureTileArrowDown;
        else if (tileSelected.flowDirectionX == -1 && tileSelected.flowDirectionY == 1)
            textureSelected = textureTileArrowDownLeft;
        else if (tileSelected.flowDirectionX == -1 && tileSelected.flowDirectionY == 0)
            textureSelected = textureTileArrowLeft;
        else if (tileSelected.flowDirectionX == -1 && tileSelected.flowDirectionY == -1)
            textureSelected = textureTileArrowUpLeft;
    }

    //Draw the tile.
    if (textureSelected != nullptr) {
        SDL_Rect rect = { x * tileSize, y * tileSize, tileSize, tileSize };
        SDL_RenderCopy(renderer, textureSelected, NULL, &rect);
    }
}

Vector2D Level::getRandomEnemySpawnerLocation() {
    //Create a list of all tiles that are enemy spawners.
    std::vector<int> listSpawnerIndices;
    for (int count = 0; count < listTiles.size(); count++) {
        auto& tileSelected = listTiles[count];
        if (tileSelected.type == TileType::enemySpawner)
            listSpawnerIndices.push_back(count);
    }

    //If one or more spawners are found, pick one at random and output it's center position.
    if (listSpawnerIndices.empty() == false) {
        int index = listSpawnerIndices[rand() % listSpawnerIndices.size()];
        return Vector2D((float)(index % tileCountX) + GameConfig::TILE_CENTER_OFFSET,
            (float)(index / tileCountX) + GameConfig::TILE_CENTER_OFFSET);
    }

    return Vector2D(GameConfig::TILE_CENTER_OFFSET, GameConfig::TILE_CENTER_OFFSET);
}

bool Level::isTileWall(int x, int y) {
    return (getTileType(x, y) == TileType::wall);
}

bool Level::isTileSpawner(int x, int y) {
    return (getTileType(x, y) == TileType::enemySpawner);
}

bool Level::hasTarget() const {
    return targetPlaced;
}

bool Level::hasSpawners() const {
    for (const auto& tileSelected : listTiles) {
        if (tileSelected.type == TileType::enemySpawner)
            return true;
    }
    return false;
}

bool Level::isTargetTile(int x, int y) const {
    return targetPlaced && x == targetX && y == targetY;
}

void Level::setTileWall(int x, int y, bool setWall) {
    if (getTileType(x, y) == TileType::enemySpawner)
        return;
    if (targetPlaced && x == targetX && y == targetY)
        return;

    setTileType(x, y, (setWall ? TileType::wall : TileType::empty));
}

void Level::setSpawner(int x, int y, bool add) {
    if (add) {
        if (getTileType(x, y) != TileType::empty)
            return;
        if (targetPlaced && x == targetX && y == targetY)
            return;
        setTileType(x, y, TileType::enemySpawner);
    }
    else if (getTileType(x, y) == TileType::enemySpawner) {
        setTileType(x, y, TileType::empty);
    }
}

void Level::setTargetPosition(int x, int y) {
    if (targetPlaced)
        return;

    if (getTileType(x, y) == TileType::wall ||
        getTileType(x, y) == TileType::enemySpawner)
        return;

    targetX = x;
    targetY = y;
    targetPlaced = true;
    calculateFlowField();
}

void Level::clearTarget() {
    if (!targetPlaced)
        return;

    targetPlaced = false;
    for (auto& tileSelected : listTiles) {
        tileSelected.flowDirectionX = 0;
        tileSelected.flowDirectionY = 0;
        tileSelected.flowDistance = flowDistanceMax;
    }
}

void Level::collectLayout(std::vector<std::pair<int, int>>& wallsOut,
    std::vector<std::pair<int, int>>& spawnersOut,
    int& targetXOut, int& targetYOut) const {
    wallsOut.clear();
    spawnersOut.clear();

    for (int y = 0; y < tileCountY; y++) {
        for (int x = 0; x < tileCountX; x++) {
            int index = x + y * tileCountX;
            if (listTiles[index].type == TileType::wall)
                wallsOut.push_back({ x, y });
            else if (listTiles[index].type == TileType::enemySpawner)
                spawnersOut.push_back({ x, y });
        }
    }

    targetXOut = targetX;
    targetYOut = targetY;
}

Level::TileType Level::getTileType(int x, int y) {
    int index = x + y * tileCountX;
    if (index > -1 && index < listTiles.size() &&
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY)
        return listTiles[index].type;

    return TileType::empty;
}

void Level::setTileTypeWithoutRecalc(int x, int y, TileType tileType) {
    int index = x + y * tileCountX;
    if (index > -1 && index < (int)listTiles.size() &&
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY)
        listTiles[index].type = tileType;
}

void Level::setTileType(int x, int y, TileType tileType) {
    int index = x + y * tileCountX;
    if (index > -1 && index < listTiles.size() &&
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY) {
        listTiles[index].type = tileType;
        calculateFlowField();
    }
}

Vector2D Level::getTargetPos() {
    return Vector2D((float)targetX + GameConfig::TILE_CENTER_OFFSET,
        (float)targetY + GameConfig::TILE_CENTER_OFFSET);
}

void Level::calculateFlowField() {
    if (!targetPlaced)
        return;

    //Ensure the target is in bounds.
    int indexTarget = targetX + targetY * tileCountX;
    if (indexTarget > -1 && indexTarget < listTiles.size() &&
        targetX > -1 && targetX < tileCountX &&
        targetY > -1 && targetY < tileCountY) {

        //Reset the tile flow data.
        for (auto& tileSelected : listTiles) {
            tileSelected.flowDirectionX = 0;
            tileSelected.flowDirectionY = 0;
            tileSelected.flowDistance = flowDistanceMax;
        }

        //Calculate the flow field.
        calculateDistances();
        calculateFlowDirections();
    }
}

void Level::calculateDistances() {
    int indexTarget = targetX + targetY * tileCountX;

    //Create a queue that will contain the indices to be checked.
    std::queue<int> listIndicesToCheck;
    //Set the target tile's flow value to 0 and add it to the queue.
    listTiles[indexTarget].flowDistance = 0;
    listIndicesToCheck.push(indexTarget);

    //The offset of the neighboring tiles to be checked.
    const int listNeighbors[][2] = { { -1, 0}, {1, 0}, {0, -1}, {0, 1} };

    //Loop through the queue and assign distance to each tile.
    while (listIndicesToCheck.empty() == false) {
        int indexCurrent = listIndicesToCheck.front();
        listIndicesToCheck.pop();

        //Check each of the neighbors;
        for (int count = 0; count < 4; count++) {
            int neighborX = listNeighbors[count][0] + indexCurrent % tileCountX;
            int neighborY = listNeighbors[count][1] + indexCurrent / tileCountX;
            int indexNeighbor = neighborX + neighborY * tileCountX;

            //Ensure that the neighbor exists and isn't a wall.
            if (indexNeighbor > -1 && indexNeighbor < listTiles.size() &&
                neighborX > -1 && neighborX < tileCountX &&
                neighborY > -1 && neighborY < tileCountY &&
                listTiles[indexNeighbor].type != TileType::wall) {

                //Check if the tile has been assigned a distance yet or not.
                if (listTiles[indexNeighbor].flowDistance == flowDistanceMax) {
                    //If not the set it's distance and add it to the queue.
                    listTiles[indexNeighbor].flowDistance = listTiles[indexCurrent].flowDistance + 1;
                    listIndicesToCheck.push(indexNeighbor);
                }
            }
        }
    }
}

void Level::calculateFlowDirections() {
    //The offset of the neighboring tiles to be checked.
    const int listNeighbors[][2] = {
        {-1, 0}, {-1, 1}, {0, 1}, {1, 1},
        {1, 0}, {1, -1}, {0, -1}, {-1, -1} };

    for (int indexCurrent = 0; indexCurrent < listTiles.size(); indexCurrent++) {
        //Ensure that the tile has been assigned a distance value.
        if (listTiles[indexCurrent].flowDistance != flowDistanceMax) {
            //Set the best distance to the current tile's distance.
            unsigned char flowFieldBest = listTiles[indexCurrent].flowDistance;

            //Check each of the neighbors;
            for (int count = 0; count < 8; count++) {
                int offsetX = listNeighbors[count][0];
                int offsetY = listNeighbors[count][1];

                int neighborX = offsetX + indexCurrent % tileCountX;
                int neighborY = offsetY + indexCurrent / tileCountX;
                int indexNeighbor = neighborX + neighborY * tileCountX;

                //Ensure that the neighbor exists.
                if (indexNeighbor > -1 && indexNeighbor < listTiles.size() &&
                    neighborX > -1 && neighborX < tileCountX &&
                    neighborY > -1 && neighborY < tileCountY) {
                    //If the current neighbor's distance is lower than the best then use it.
                    if (listTiles[indexNeighbor].flowDistance < flowFieldBest) {
                        flowFieldBest = listTiles[indexNeighbor].flowDistance;
                        listTiles[indexCurrent].flowDirectionX = offsetX;
                        listTiles[indexCurrent].flowDirectionY = offsetY;
                    }
                }
            }
        }
    }
}

Vector2D Level::getFlowNormal(int x, int y) {
    int index = x + y * tileCountX;
    if (index > -1 && index < listTiles.size() &&
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY)
        return Vector2D((float)listTiles[index].flowDirectionX, (float)listTiles[index].flowDirectionY).normalize();

    return Vector2D();
}
