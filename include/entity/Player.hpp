#pragma once

#include <SFML/Graphics.hpp>
#include "core/KeyHandler.hpp"
#include "Entity.hpp"

class Player : public Entity
{
public:
    Player(const GameConfig &config, const KeyHandler& keyHandler);

    void updateDirection() override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window, bool showDebug) override;

private:
    bool isMoving;
    int spriteNum;
    float animationTimer;

    sf::Texture texture1;
    sf::Texture texture2;

    const KeyHandler &keyHandler;
    void setDefaultValues();

};