#include <iostream>
#include "core/CollisionChecker.hpp"
#include "entity/Entity.hpp"
#include <cmath>

CollisionChecker::CollisionChecker(
    const GameConfig& config, 
    const MapManager& mapManager
) : config(config),
    mapManager(mapManager)
{
}

void CollisionChecker::checkCollision(Entity& entity, float deltaTime)
{
    entity.collisionOn = false;

    float entityLeftWorldX   = entity.worldX;
    float entityRightWorldX  = entity.worldX + config.tileSize - 1;
    float entityTopWorldY    = entity.worldY;
    float entityBottomWorldY = entity.worldY + config.tileSize - 1;

    int entityLeftCol        = static_cast<int>(std::floor(entityLeftWorldX   / config.tileSize));
    int entityRightCol       = static_cast<int>(std::floor(entityRightWorldX  / config.tileSize));
    int entityTopRow         = static_cast<int>(std::floor(entityTopWorldY    / config.tileSize));
    int entityBottomRow      = static_cast<int>(std::floor(entityBottomWorldY / config.tileSize));

    auto isObjectCollidable = [this](int col, int row)
    {
        // CHECK IF IT'S END OF THE MAP
        if (col < 0 || col >= config.maxWorldCol ||
            row < 0 || row >= config.maxWorldRow)
        {
            return true;
        }

        int index = col + row * config.maxWorldCol;
        int objectType = mapManager.objectMap[index];

        // CHECK AGAINST INVALID OBJECT IDs
        if (objectType < 0 || objectType >= static_cast<int>(mapManager.objectTypes.size()))
        {
            return true;
        }

        return mapManager.objectTypes[objectType].collidable;
    };

    switch(entity.direction)
    {
        case Direction::Up:
            entityTopRow = static_cast<int>(
                std::floor((entityTopWorldY - entity.speed * deltaTime) / config.tileSize)
            );

            if (isObjectCollidable(entityLeftCol, entityTopRow) ||
                isObjectCollidable(entityRightCol, entityTopRow))
            {
                entity.worldY = entityTopRow * config.tileSize + config.tileSize;
                entity.collisionOn = true;
            }
            break;

        case Direction::Down:
            entityBottomRow = static_cast<int>(
                std::floor((entityBottomWorldY + entity.speed * deltaTime) / config.tileSize)
            );

            if (isObjectCollidable(entityLeftCol, entityBottomRow) ||
                isObjectCollidable(entityRightCol, entityBottomRow))
            {
                entity.worldY = entityBottomRow * config.tileSize - config.tileSize;
                entity.collisionOn = true;
            }
            break;
    
        case Direction::Left:
            entityLeftCol = static_cast<int>(
                std::floor((entityLeftWorldX - entity.speed * deltaTime) / config.tileSize)
            );
            
            if (isObjectCollidable(entityLeftCol, entityTopRow) ||
                isObjectCollidable(entityLeftCol, entityBottomRow))
            {
                entity.worldX = entityLeftCol * config.tileSize + config.tileSize;
                entity.collisionOn = true;
            }
            break;

        case Direction::Right:
            entityRightCol = static_cast<int>(
                std::floor((entityRightWorldX + entity.speed * deltaTime) / config.tileSize)
            );

            if (isObjectCollidable(entityRightCol, entityTopRow) ||
                isObjectCollidable(entityRightCol, entityBottomRow))
            {
                entity.worldX = entityRightCol * config.tileSize - config.tileSize;
                entity.collisionOn = true;
            }
            break;

        case Direction::None:
            break;
    }
}
