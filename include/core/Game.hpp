#pragma once
#include <SFML/Graphics.hpp>
#include "tile/TileManager.hpp"
#include "core/KeyHandler.hpp"
#include "entity/Player.hpp"

class Game {
public:
    Game();
    void run();

private:
    const int ORIGINAL_TILE_SIZE = 16;
    const int SCALE = 3;

    const int TILE_SIZE = ORIGINAL_TILE_SIZE * SCALE;

    const int MAX_WORLD_COL = 40;
    const int MAX_WORLD_ROW = 40;
    const int MAX_SCREEN_COL = 16;
    const int MAX_SCREEN_ROW = 12;

    const unsigned int SCREEN_WIDTH = TILE_SIZE * MAX_SCREEN_COL;
    const unsigned int SCREEN_HEIGHT = TILE_SIZE * MAX_SCREEN_ROW;

    const int FPS = 60;

    sf::RenderWindow window;
    sf::View camera;

    GameConfig config;
    TileManager tileManager;
    KeyHandler keyHandler;
    Player player;

    void setupGame();
    void update(float deltaTime);
    void render();
};