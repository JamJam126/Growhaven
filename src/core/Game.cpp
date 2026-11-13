#include "core/Game.hpp"
#include <stdio.h>
#include <algorithm>
#include <cmath>

Game::Game()
  : window(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "Growhaven"),
    config {
        TILE_SIZE,
        SCALE,
        MAX_WORLD_COL,
        MAX_WORLD_ROW,
        MAX_SCREEN_COL,
        MAX_SCREEN_ROW
    },
    mapManager(config),
    keyHandler(),
    cChecker(config, mapManager),
    player(config, keyHandler)
{
    window.setFramerateLimit(FPS);
}

void Game::run() {
    setupGame();
    sf::Clock clock;

    while(window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        while (const std::optional<sf::Event> event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}

            // MATCH CAMERA TO WINDOW SIZE (SHOW MORE WORLD INSTEAD OF STRETCHING)
            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                camera.setSize(sf::Vector2f(
                    static_cast<float>(resized->size.x),
                    static_cast<float>(resized->size.y)
                ));
                updateCamera();
            }

            keyHandler.handleKeyEvent(*event);
        }

		update(deltaTime);
        render();
    }
}

void Game::setupGame() {

    camera.setSize(sf::Vector2f(
        static_cast<float>(SCREEN_WIDTH),
        static_cast<float>(SCREEN_HEIGHT)
    ));
    
    // INITIALIZE CAMERA POSITION
    updateCamera();
}

void Game::update(float deltaTime) {

    // UPDATE PLAYER DIRECTION
    player.updateDirection();

    // CHECK PLAYER COLLISION
    cChecker.checkCollision(player, deltaTime);
    
    // UPDATE PLAYER
    player.update(deltaTime);

    // UPDATE CAMERA
    updateCamera();
}

void Game::updateCamera() {
    const sf::Vector2f viewSize = camera.getSize();

    // FOLLOW PLAYER, BUT DON'T SHOW PAST THE WORLD EDGES.
    // IF THE VIEW IS BIGGER THAN THE WORLD, CENTER THE WORLD INSTEAD.
    auto clampAxis = [](float target, float view, float world) {
        if (view >= world) {
            return world / 2.0f;
        }
        return std::clamp(target, view / 2.0f, world - view / 2.0f);
    };

    cameraX = clampAxis(player.worldX + config.tileSize / 2.0f, viewSize.x, static_cast<float>(WORLD_WIDTH));
    cameraY = clampAxis(player.worldY + config.tileSize / 2.0f, viewSize.y, static_cast<float>(WORLD_HEIGHT));

    // SNAP VIEW'S TOP-LEFT TO A WHOLE PIXEL SO TILES STAY CRISP
    float left = std::round(cameraX - viewSize.x / 2.0f);
    float top  = std::round(cameraY - viewSize.y / 2.0f);

    camera.setCenter(sf::Vector2f(
        left + viewSize.x / 2.0f,
        top  + viewSize.y / 2.0f
    ));
}

void Game::render() {
    window.clear();
    window.setView(camera);

    mapManager.render(window, keyHandler.showDebug);
    player.render(window, keyHandler.showDebug);
    
    window.display();
}
