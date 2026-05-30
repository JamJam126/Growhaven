#pragma once
#include <SFML/Graphics.hpp>
#include "core/GameConfig.hpp"
#include "tile/Tile.hpp"
#include <vector>
#include <string>

class TileManager {
public:
    TileManager(const GameConfig& config);
    void render(sf::RenderWindow& window);
    
private:
    enum TileType
    {
        GRASS,
        SOIL,
        WATER
    };

    enum TileIndex
    {
        GRASS_FULL,
        GRASS_EDGE_TOP,
        GRASS_EDGE_RIGHT,
        GRASS_EDGE_BOTTOM,
        GRASS_EDGE_LEFT,
        GRASS_INNER_CORNER_TOPLEFT,
        GRASS_INNER_CORNER_TOPRIGHT,
        GRASS_INNER_CORNER_BOTTOMRIGHT,
        GRASS_INNER_CORNER_BOTTOMLEFT,
        GRASS_OUTER_CORNER_TOPLEFT,
        GRASS_OUTER_CORNER_TOPRIGHT,
        GRASS_OUTER_CORNER_BOTTOMRIGHT,
        GRASS_OUTER_CORNER_BOTTOMLEFT,

        SOIL_FULL
    };

    enum TileMask
    {
        MASK_FULL = 15,
        MASK_EDGE_TOP = 62,
        MASK_EDGE_RIGHT = 253,
        MASK_EDGE_BOTTOM = 203,
        MASK_EDGE_LEFT = 247,

        MASK_INNER_CORNER_TOPLEFT = 239,
        MASK_INNER_CORNER_TOPRIGHT = 223,
        MASK_INNER_CORNER_BOTTOMRIGHT = 191,
        MASK_INNER_CORNER_BOTTOMLEFT = 127,
    };

    GameConfig config;
    std::vector<Tile> tiles;
    std::vector<std::vector<int>> tileset;
    std::vector<int> map;
    std::vector<int> tileIndices;

    void getTileImage();
    void setup(int index, const std::string& imageName, bool collision);
    void setTileset(int tileType, int mask, int tileIndex);
    void loadMap(const std::string& filePath);
    int calculateMask(int type, int x, int y);
    bool same(int x, int y, int type);
    int getTileIndex(int x, int y);
};