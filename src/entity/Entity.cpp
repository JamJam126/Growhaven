#include <iostream>
#include <SFML/Graphics.hpp>
#include "entity/Entity.hpp"

Entity::Entity(const GameConfig &config) : config(config)
{
    collisionOn = false;
}

void Entity::update(float deltaTime)
{
    switch (direction)
    {
    case Direction::Up:
        std::cout << "Up" << std::endl;
        break;

    case Direction::Down:
        std::cout << "Down" << std::endl;
        break;

    case Direction::Left:
        std::cout << "Left" << std::endl;
        break;

    case Direction::Right:
        std::cout << "Right" << std::endl;
        break;
    }
}

void Entity::render(sf::RenderWindow &window, bool showDebug)
{
    
}
