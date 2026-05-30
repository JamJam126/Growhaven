#pragma once
#include <SFML/Graphics.hpp>

struct Tile 
{
    sf::Texture texture;
    bool collision = false;
};
