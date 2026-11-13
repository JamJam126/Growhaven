#pragma once
#include "core/GameConfig.hpp"
#include "map/MapManager.hpp"
#include "entity/Entity.hpp"

class CollisionChecker {
public:
    CollisionChecker(const GameConfig& config, const MapManager& mapManager);
    void checkCollision(Entity& entity, float deltaTime);

private:
    GameConfig config;
    const MapManager& mapManager;
};
