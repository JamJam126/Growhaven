#pragma once
#include <SFML/Graphics.hpp>
#include "core/GameConfig.hpp"
#include "map/Tile.hpp"
#include "map/Object.hpp"
#include "map/ObjectType.hpp"
#include <vector>
#include <string>

class MapManager {
public:
    // TILE DATA
    std::vector<Tile> tiles;
    std::vector<std::vector<int>> tileset;
    std::vector<int> tileMap;
    std::vector<int> tileIndices;

    // OBJECT DATA
    std::vector<Object> objects;
    std::vector<ObjectType> objectTypes;
    std::vector<int> objectMap;
    
    MapManager(const GameConfig& config);
    void render(sf::RenderWindow& window);
    
private:

    // TILE IDs
    enum TileType
    {
        GRASS,
        SOIL,
        WATER
    };

    // LOADED TILE TEXTURE SLOTS
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

    // OBJECT IDs
    enum ObjectIndex
    {
        EMPTY,
        TREE    
    };

    GameConfig config;

    // SETUP
    void loadTileTextures();
    void loadObjectTypes();
    void setupTile(int index, const std::string& imageName);
    void setupObject(int index, const std::string &imageName, bool collidable = true);    
    
    // LOADING
    void loadTileMap(const std::string &filePath);
    void loadObjectMap(const std::string &filePath);

    // TILE HELPERS
    void setTileset(int tileType, int mask, int tileIndex);
    int calculateMask(int type, int x, int y);
    bool isSameTileType(int x, int y, int type);
    int getTileIndex(int x, int y);
};