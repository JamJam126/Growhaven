#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "tile/TileManager.hpp"
#include "tile/Tile.hpp"

TileManager::TileManager(const GameConfig &config) : config(config)
{
    getTileImage();
    loadMap("res/maps/map01.txt");
}

void TileManager::getTileImage()
{
    std::cout << "Tile Images\n";
    tiles.resize(30);
    tileset.resize(5, std::vector<int>(256));
    setup(SOIL_FULL, "soil", false);

    // GRASS TEXTURE
    setup(GRASS_FULL, "grass/grass-00", false);
    setup(GRASS_EDGE_TOP, "grass/edge_top", true);
    setup(GRASS_EDGE_RIGHT, "grass/edge_right", true);
    setup(GRASS_EDGE_BOTTOM, "grass/edge_bottom", true);
    setup(GRASS_EDGE_LEFT, "grass/edge_left", true);
    setup(GRASS_INNER_CORNER_TOPLEFT, "grass/inner_corner_topleft", false);
    setup(GRASS_INNER_CORNER_TOPRIGHT, "grass/inner_corner_topright", false);
    setup(GRASS_INNER_CORNER_BOTTOMRIGHT, "grass/inner_corner_bottomright", false);
    setup(GRASS_INNER_CORNER_BOTTOMLEFT, "grass/inner_corner_bottomleft", false);
    setup(GRASS_OUTER_CORNER_TOPLEFT, "grass/outer_corner_topleft", false);
    setup(GRASS_OUTER_CORNER_TOPRIGHT, "grass/outer_corner_topright", false);
    setup(GRASS_OUTER_CORNER_BOTTOMRIGHT, "grass/outer_corner_bottomright", false);
    setup(GRASS_OUTER_CORNER_BOTTOMLEFT, "grass/outer_corner_bottomleft", false);

    setTileset(SOIL, MASK_FULL, SOIL_FULL);

    setTileset(GRASS, MASK_FULL, GRASS_FULL);

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

    setTileset(GRASS, MASK_INNER_CORNER_TOPLEFT, GRASS_INNER_CORNER_TOPLEFT);
    setTileset(GRASS, MASK_INNER_CORNER_TOPRIGHT, GRASS_INNER_CORNER_TOPRIGHT);
    setTileset(GRASS, MASK_INNER_CORNER_BOTTOMRIGHT, GRASS_INNER_CORNER_BOTTOMRIGHT);
    setTileset(GRASS, MASK_INNER_CORNER_BOTTOMLEFT, GRASS_INNER_CORNER_BOTTOMLEFT);

    setTileset(GRASS, 240, GRASS_OUTER_CORNER_TOPLEFT);
    setTileset(GRASS, 70,  GRASS_OUTER_CORNER_TOPLEFT);
    setTileset(GRASS, 70,  GRASS_OUTER_CORNER_TOPLEFT);

    setTileset(GRASS, 140, GRASS_OUTER_CORNER_TOPRIGHT);
    setTileset(GRASS, 25,  GRASS_OUTER_CORNER_BOTTOMRIGHT);
    setTileset(GRASS, 35,  GRASS_OUTER_CORNER_BOTTOMLEFT);
}

void TileManager::setup(int index, const std::string &imageName, bool collision)
{
    if (!tiles[index].texture.loadFromFile("res/tiles/" + imageName + ".png"))
    {
        throw std::runtime_error("Failed to load texture: " + imageName);
    }

    tiles[index].collision = collision;
}

void TileManager::setTileset(int tileType, int mask, int tileIndex)
{
    tileset[tileType][mask] = tileIndex;
}

void TileManager::loadMap(const std::string &filePath)
{
    std::fstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open map file: " + filePath);
    }

    map.resize(config.maxWorldCol * config.maxWorldRow);
    tileIndices.resize(config.maxWorldCol * config.maxWorldRow);

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
            map.at(index) = num;
            col++;
        }

        row++;
    }

    for (int y = 0; y < config.maxWorldRow; y++)
    {
        for (int x = 0; x < config.maxWorldCol; x++)
        {
            int index = x + y * config.maxWorldCol;
            int type = map[index];
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

void TileManager::render(sf::RenderWindow &window)
{
    for (int y = 0; y < config.maxWorldRow; y++)
    {
        for (int x = 0; x < config.maxWorldCol; x++)
        {
            sf::Sprite sprite(tiles[getTileIndex(x, y)].texture);
            sf::Vector2f pos(
                x * config.tileSize,
                y * config.tileSize
            );
            sf::Vector2f scale(
                config.scale,
                config.scale
            );

            sprite.setPosition(pos);
            sprite.setScale(scale);

            window.draw(sprite);
        }
    }
}

int TileManager::calculateMask(int type, int x, int y)
{
    if (type == SOIL)
        return 15; 

    bool up    = same(x, y - 1, type);
    bool right = same(x + 1, y, type);
    bool down  = same(x, y + 1, type);
    bool left  = same(x - 1, y, type);

    bool topLeft     = up && left && same(x - 1, y - 1, type);
    bool topRight    = up && right && same(x + 1, y - 1, type);
    bool bottomRight = down && right && same(x + 1, y + 1, type);
    bool bottomLeft  = down && left && same(x - 1, y + 1, type);

    int mask = 0;

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

bool TileManager::same(int x, int y, int type)
{
    if (x < 0 || x >= config.maxWorldCol ||
        y < 0 || y >= config.maxWorldRow)
        return true;

    return map[x + y * config.maxWorldCol] == type;
}

int TileManager::getTileIndex(int x, int y)
{
    return tileIndices[x + y * config.maxWorldCol];
}
