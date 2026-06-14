#include "Game.h"

Game::Game(SDL_Window* window, SDL_Renderer* renderer) :
    placementModeCurrent(PlacementMode::wall), 
    level(renderer, GameConfig::TILE_COUNT_X, GameConfig::TILE_COUNT_Y) {

    //Run the game.
    if (window != nullptr && renderer != nullptr) {
        //Load the overlay texture.
        textureOverlay = TextureLoader::loadTexture(renderer, "Overlay.bmp");

        //Store the current times for the clock.
        auto time1 = std::chrono::system_clock::now();
        auto time2 = std::chrono::system_clock::now();

        //The amount of time for each frame (60 fps).
        const float dT = 1.0f / 60.0f;

        //Start the game loop and run until it's time to stop.
        bool running = true;
        while (running) {
            //Determine how much time has elapsed since the last frame.
            time2 = std::chrono::system_clock::now();
            std::chrono::duration<float> timeDelta = time2 - time1;
            float timeDeltaFloat = timeDelta.count();

            //If enough time has passed then do everything required to generate the next frame.
            if (timeDeltaFloat >= dT) {
                //Store the new time for the next frame.
                time1 = time2;

                processEvents(renderer, running);
                update(dT);
                draw(renderer);
            }
        }
    }
}

Game::~Game() {
    //Clean up.
    TextureLoader::deallocateTextures();
}

void Game::processEvents(SDL_Renderer* renderer, bool& running) {
    bool mouseDownThisFrame = false;

    //Process events.
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            mouseDownThisFrame = (mouseDownStatus == 0);
            if (event.button.button == SDL_BUTTON_LEFT)
                mouseDownStatus = SDL_BUTTON_LEFT;
            else if (event.button.button == SDL_BUTTON_RIGHT)
                mouseDownStatus = SDL_BUTTON_RIGHT;
            break;
        case SDL_MOUSEBUTTONUP:
            mouseDownStatus = 0;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
                //Quit the game.
            case SDL_SCANCODE_ESCAPE:
                running = false;
                break;

                //Set the current gamemode.
            case SDL_SCANCODE_1:
                placementModeCurrent = PlacementMode::wall;
                break;
            case SDL_SCANCODE_2:
                placementModeCurrent = PlacementMode::unit;
                break;

                //Show/hide the overlay
            case SDL_SCANCODE_H:
                overlayVisible = !overlayVisible;
                break;
            }
        }
    }

    //Process input from the mouse cursor.
    int mouseX = 0, mouseY = 0;
    SDL_GetMouseState(&mouseX, &mouseY);

    //Convert from the window's coordinate system to the game's coordinate system.
    Vector2D posMouse((float)mouseX / GameConfig::TILE_SIZE, (float)mouseY / GameConfig::TILE_SIZE);

    if (mouseDownStatus > 0) {
        switch (mouseDownStatus) {
        case SDL_BUTTON_LEFT:
            switch (placementModeCurrent) {
            case PlacementMode::wall:

                //Add wall at the mouse position.
                level.setTileWall((int)posMouse.x, (int)posMouse.y, true);
                break;
            case PlacementMode::unit:

                //Add the selected unit at the mouse position.
                if (mouseDownThisFrame)
                    addUnit(renderer, posMouse);
                break;
            }
            break;

        case SDL_BUTTON_RIGHT:

            //Remove wall at the mouse position.
            level.setTileWall((int)posMouse.x, (int)posMouse.y, false);

            //Remove units at the mouse position.
            removeUnitsAtMousePosition(posMouse);
            break;
        }
    }
}

void Game::update(float dT) {
    //Update the units.
    for (auto& unitSelected : listUnits)
        unitSelected.update(dT, level, listUnits);
}

void Game::draw(SDL_Renderer* renderer) {
    //Draw.
    //Set the draw color to white.
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //Clear the screen.
    SDL_RenderClear(renderer);

    //Draw everything here.
    //Draw the level.
    level.draw(renderer, GameConfig::TILE_SIZE);

    //Draw the enemy units.
    for (auto& unitSelected : listUnits)
        unitSelected.draw(renderer, GameConfig::TILE_SIZE);
    
    //Draw the overlay.
    if (textureOverlay != nullptr && overlayVisible) {
        int w = 0, h = 0;
        SDL_QueryTexture(textureOverlay, NULL, NULL, &w, &h);
        SDL_Rect rect = { 40, 40, w, h };
        SDL_RenderCopy(renderer, textureOverlay, NULL, &rect);
    }

    //Send the image to the window.
    SDL_RenderPresent(renderer);
}

void Game::addUnit(SDL_Renderer* renderer, Vector2D posMouse) {
    listUnits.push_back(Unit(renderer, posMouse));
}

void Game::removeUnitsAtMousePosition(Vector2D posMouse) {
    for (int count = 0; count < listUnits.size(); count++) {
        auto& unitSelected = listUnits[count];
        if (unitSelected.checkOverlap(posMouse, 0.0f)) {
            listUnits.erase(listUnits.begin() + count);
            count--;
        }
    }
}