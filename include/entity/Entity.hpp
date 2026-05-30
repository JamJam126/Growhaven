#pragma once

#include <string>
#include "core/GameConfig.hpp"

enum class Direction
{
    Up,
    Down,
    Left,
    Right,
    None
};

class Entity {
public:
    float worldX, worldY;
    float speed;

    Direction direction;
    GameConfig config;

    Entity(const GameConfig &config);

    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow &window) = 0;

    virtual ~Entity() = default;

private:
};