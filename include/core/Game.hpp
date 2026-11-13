#pragma once
#include <SFML/Graphics.hpp>
#include "core/KeyHandler.hpp"
#include "core/CollisionChecker.hpp"
#include "map/MapManager.hpp"
#include "entity/Player.hpp"

class Game {
public:
    Game();
    void run();

private:
    const int ORIGINAL_TILE_SIZE = 16;
    const int SCALE = 3;

    const int TILE_SIZE = ORIGINAL_TILE_SIZE * SCALE;

    const int MAX_WORLD_COL  = 40;
    const int MAX_WORLD_ROW  = 40;
    const int MAX_SCREEN_COL = 17;
    const int MAX_SCREEN_ROW = 13;

    const unsigned int WORLD_WIDTH   = TILE_SIZE * MAX_WORLD_COL;
    const unsigned int WORLD_HEIGHT  = TILE_SIZE * MAX_WORLD_ROW;
    const unsigned int SCREEN_WIDTH  = TILE_SIZE * MAX_SCREEN_COL;
    const unsigned int SCREEN_HEIGHT = TILE_SIZE * MAX_SCREEN_ROW;

    const int FPS = 60;

    sf::RenderWindow window;
    sf::View camera;

    GameConfig config;
    MapManager mapManager;
    KeyHandler keyHandler;
    CollisionChecker cChecker;

    Player player;

    int cameraX;
    int cameraY;

    void setupGame();
    void update(float deltaTime);
    void render();
};