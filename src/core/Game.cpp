#include "core/Game.hpp"
#include <stdio.h>

Game::Game()
  : window(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "Growhaven"),
    config{
        TILE_SIZE,
        SCALE,
        MAX_WORLD_COL,
        MAX_WORLD_ROW,
        MAX_SCREEN_COL,
        MAX_SCREEN_ROW},
    tileManager(config),
    keyHandler(),
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
        static_cast<float>(SCREEN_HEIGHT))
    );
    camera.setCenter(sf::Vector2f(
        player.worldX * config.tileSize - config.tileSize / 2,
        player.worldY * config.tileSize - config.tileSize / 2
    ));
}

void Game::update(float deltaTime) {
    player.update(deltaTime);
    camera.setCenter(sf::Vector2f(
        player.worldX * config.tileSize - config.tileSize / 2, 
        player.worldY * config.tileSize - config.tileSize / 2
    ));
}

void Game::render() {
    window.clear();
    window.setView(camera);

    tileManager.render(window);
    player.render(window);
    
    window.display();
}
