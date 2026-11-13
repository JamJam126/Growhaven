#include <string>
#include <sstream>
#include <fstream>
#include "map/MapManager.hpp"
#include "map/Tile.hpp"

MapManager::MapManager(const GameConfig &config) : config(config)
{
    loadTileTextures();
    loadObjectTypes();
    loadTileMap("res/maps/tile01.txt");
    loadObjectMap("res/maps/object01.txt");
}

void MapManager::loadTileTextures()
{
    tiles.resize(30);
    tileset.resize(5, std::vector<int>(256));

    // TILE TEXTURES
    setupTile(SOIL_FULL, "soil");
    setupTile(GRASS_FULL, "grass/grass-00");
    setupTile(GRASS_EDGE_TOP, "grass/edge_top");
    setupTile(GRASS_EDGE_RIGHT, "grass/edge_right");
    setupTile(GRASS_EDGE_BOTTOM, "grass/edge_bottom");
    setupTile(GRASS_EDGE_LEFT, "grass/edge_left");
    setupTile(GRASS_INNER_CORNER_TOPLEFT, "grass/inner_corner_topleft");
    setupTile(GRASS_INNER_CORNER_TOPRIGHT, "grass/inner_corner_topright");
    setupTile(GRASS_INNER_CORNER_BOTTOMRIGHT, "grass/inner_corner_bottomright");
    setupTile(GRASS_INNER_CORNER_BOTTOMLEFT, "grass/inner_corner_bottomleft");
    setupTile(GRASS_OUTER_CORNER_TOPLEFT, "grass/outer_corner_topleft");
    setupTile(GRASS_OUTER_CORNER_TOPRIGHT, "grass/outer_corner_topright");
    setupTile(GRASS_OUTER_CORNER_BOTTOMRIGHT, "grass/outer_corner_bottomright");
    setupTile(GRASS_OUTER_CORNER_BOTTOMLEFT, "grass/outer_corner_bottomleft");

    // TILE MASKS
    setTileset(SOIL, 15, SOIL_FULL);

    setTileset(GRASS, 15, GRASS_FULL);

    setTileset(GRASS, 206, GRASS_EDGE_TOP);
    setTileset(GRASS, 238, GRASS_EDGE_TOP);
    setTileset(GRASS, 222, GRASS_EDGE_TOP);
    setTileset(GRASS, 254, GRASS_EDGE_TOP);

    setTileset(GRASS, 157, GRASS_EDGE_RIGHT);
    setTileset(GRASS, 221, GRASS_EDGE_RIGHT);
    setTileset(GRASS, 187, GRASS_EDGE_RIGHT);
    setTileset(GRASS, 253, GRASS_EDGE_RIGHT);

    setTileset(GRASS, 59,  GRASS_EDGE_BOTTOM);
    setTileset(GRASS, 123, GRASS_EDGE_BOTTOM);
    setTileset(GRASS, 187, GRASS_EDGE_BOTTOM);
    setTileset(GRASS, 251, GRASS_EDGE_BOTTOM);

    setTileset(GRASS, 103, GRASS_EDGE_LEFT);
    setTileset(GRASS, 231, GRASS_EDGE_LEFT);
    setTileset(GRASS, 119, GRASS_EDGE_LEFT);
    setTileset(GRASS, 247, GRASS_EDGE_LEFT);

    setTileset(GRASS, 239, GRASS_INNER_CORNER_TOPLEFT);
    setTileset(GRASS, 223, GRASS_INNER_CORNER_TOPRIGHT);
    setTileset(GRASS, 191, GRASS_INNER_CORNER_BOTTOMRIGHT);
    setTileset(GRASS, 127, GRASS_INNER_CORNER_BOTTOMLEFT);

    setTileset(GRASS, 240, GRASS_OUTER_CORNER_TOPLEFT);
    setTileset(GRASS, 70,  GRASS_OUTER_CORNER_TOPLEFT);
    setTileset(GRASS, 70,  GRASS_OUTER_CORNER_TOPLEFT);

    setTileset(GRASS, 140, GRASS_OUTER_CORNER_TOPRIGHT);
    setTileset(GRASS, 25,  GRASS_OUTER_CORNER_BOTTOMRIGHT);
    setTileset(GRASS, 35,  GRASS_OUTER_CORNER_BOTTOMLEFT);
}

void MapManager::loadObjectTypes()
{
    objectTypes.resize(5);

    // OBJECT TYPES
    setupObject(TREE, "tree", true);
}

void MapManager::setupTile(int index, const std::string &imageName)
{
    if (!tiles[index].texture.loadFromFile("res/tiles/" + imageName + ".png"))
    {
        throw std::runtime_error("Failed to load texture: " + imageName);
    }
}

void MapManager::setupObject(int index, const std::string &imageName, bool collidable)
{
    if (!objectTypes[index].texture.loadFromFile("res/objects/" + imageName + ".png"))
    {
        throw std::runtime_error("Failed to load texture: " + imageName);
    }

    objectTypes[index].collidable = collidable;
}

void MapManager::setTileset(int tileType, int mask, int tileIndex)
{
    tileset[tileType][mask] = tileIndex;
}

void MapManager::loadTileMap(const std::string &filePath)
{
    std::fstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open map file: " + filePath);
    }

    tileMap.resize(config.maxWorldCol * config.maxWorldRow);
    tileIndices.resize(config.maxWorldCol * config.maxWorldRow);

    // READ TILE MAP
    int row = 0;
    std::string line;

    while (row < config.maxWorldRow && std::getline(file, line))
    {
        std::istringstream ss(line);

        int col = 0;
        int num;

        while (ss >> num && col < config.maxWorldCol)
        {
        int index = col + row * config.maxWorldCol;
            tileMap.at(index) = num;
            col++;
        }

        row++;
    }

    // CHOOSE TILE TEXTURES
    for (int y = 0; y < config.maxWorldRow; y++)
    {
        for (int x = 0; x < config.maxWorldCol; x++)
        {
            int index = x + y * config.maxWorldCol;
            int type = tileMap[index];
            int mask = calculateMask(type, x, y);

            int tileIndex;

            if (type == SOIL)
            {
                tileIndex = SOIL_FULL;
            }
            else
            {
                tileIndex = tileset[type][mask];
            }

            if (tileIndex == 0)
            {
                tileIndex = GRASS_FULL;
            }

            tileIndices[index] = tileIndex;
        }
    }

    file.close();
}

void MapManager::loadObjectMap(const std::string &filePath)
{
    std::fstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open map file: " + filePath);
    }

    objectMap.resize(config.maxWorldCol * config.maxWorldRow);

    // READ OBJECT MAP
    int row = 0;
    std::string line;

    while (row < config.maxWorldRow && std::getline(file, line))
    {
        std::istringstream ss(line);

        int col = 0;
        int num;

        while (ss >> num && col < config.maxWorldCol)
        {
            if (num > 0)
            {
                // ADD OBJECT
                objects.push_back({
                    num,                   // OBJECT type
                    col * config.tileSize, // OBJECT worldX
                    row * config.tileSize, // OBJECT worldY
                });
            }

            int index = col + row * config.maxWorldCol;
            objectMap.at(index) = num;

            col++;
        }

        row++;
    }

    file.close();
}

void MapManager::render(sf::RenderWindow &window, bool showDebug)
{
    // TILES
    for (int y = 0; y < config.maxWorldRow; y++)
    {
        for (int x = 0; x < config.maxWorldCol; x++)
        {
            sf::Sprite sprite(tiles[getTileIndex(x, y)].texture);

            sprite.setPosition(sf::Vector2(
                static_cast<float>(x * config.tileSize), 
                static_cast<float>(y * config.tileSize)
            ));
            
            sprite.setScale(sf::Vector2(
                static_cast<float>(config.scale), 
                static_cast<float>(config.scale)
            ));

            window.draw(sprite);
        }
    }

    // OBJECTS
    for (const Object& object : objects)
    {
        sf::Sprite sprite(objectTypes[object.type].texture);

        sprite.setPosition(sf::Vector2(
            static_cast<float>(object.worldX), 
            static_cast<float>(object.worldY)
        ));

        sprite.setScale(sf::Vector2(
            static_cast<float>(config.scale), 
            static_cast<float>(config.scale)
        ));

        window.draw(sprite);

        // DEBUGGING
        if (showDebug)
        {
            sf::RectangleShape debugBox;
            debugBox.setPosition(sf::Vector2f(
                static_cast<float>(object.worldX),
                static_cast<float>(object.worldY)));
            debugBox.setSize(sf::Vector2f(config.tileSize, config.tileSize));
            debugBox.setFillColor(sf::Color::Transparent);
            debugBox.setOutlineColor(sf::Color::Red);
            debugBox.setOutlineThickness(1.0f);

            window.draw(debugBox);
        }
    }
}

int MapManager::calculateMask(int type, int x, int y)
{
    if (type == SOIL)
        return 15; 

    // NEIGHBORS
    bool up    = isSameTileType(x    , y - 1, type);
    bool right = isSameTileType(x + 1, y    , type);
    bool down  = isSameTileType(x    , y + 1, type);
    bool left  = isSameTileType(x - 1, y    , type);

    bool topLeft     = up   && left  && isSameTileType(x - 1, y - 1, type);
    bool topRight    = up   && right && isSameTileType(x + 1, y - 1, type);
    bool bottomRight = down && right && isSameTileType(x + 1, y + 1, type);
    bool bottomLeft  = down && left  && isSameTileType(x - 1, y + 1, type);

    int mask = 0;

    // MASK
    if (up)          mask |= 1;
    if (right)       mask |= 2;
    if (down)        mask |= 4;
    if (left)        mask |= 8;

    if (topLeft)     mask |= 16;
    if (topRight)    mask |= 32;
    if (bottomRight) mask |= 64;
    if (bottomLeft)  mask |= 128;

    // std::cout << "type at " << x << ", " << y << ": " << type << std::endl;
    return mask;
}

bool MapManager::isSameTileType(int x, int y, int type)
{
    if (x < 0 || x >= config.maxWorldCol ||
        y < 0 || y >= config.maxWorldRow)
        return true;

    return tileMap[x + y * config.maxWorldCol] == type;
}

int MapManager::getTileIndex(int x, int y)
{
    return tileIndices[x + y * config.maxWorldCol];
}
