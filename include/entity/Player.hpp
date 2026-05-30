#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include "core/KeyHandler.hpp"

class Player : public Entity
{
public:
    Player(const GameConfig &config, const KeyHandler& keyHandler);

    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;

private:
    int spriteNum;
    float animationTimer;

    sf::Texture texture1;
    sf::Texture texture2;

    const KeyHandler &keyHandler;
    void setDefaultValues();

};