#include "core/Game.hpp"
#include <stdio.h>
#include <algorithm>

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
    cameraX = player.worldX - config.tileSize / 2;
    cameraY = player.worldY - config.tileSize / 2;

    camera.setCenter(sf::Vector2f(
        cameraX,
        cameraY
    ));
}

void Game::update(float deltaTime) {

    // UPDATE PLAYER DIRECTION
    player.updateDirection();

    // CHECK PLAYER COLLISION
    cChecker.checkCollision(player, deltaTime);
    
    // UPDATE PLAYER
    player.update(deltaTime);

    // UPDATE CAMERA
    cameraX = std::clamp(
        player.worldX + config.tileSize / 2.0f,
        SCREEN_WIDTH / 2.0f,
        WORLD_WIDTH - (SCREEN_WIDTH / 2.0f)
    );

    cameraY = std::clamp(
        player.worldY + config.tileSize / 2.0f,
        SCREEN_HEIGHT / 2.0f,
        WORLD_HEIGHT - (SCREEN_HEIGHT / 2.0f)
    );
    
    camera.setCenter(sf::Vector2f(
        cameraX, 
        cameraY
    ));
}

void Game::render() {
    window.clear();
    window.setView(camera);

    mapManager.render(window);
    player.render(window);
    
    window.display();
}
