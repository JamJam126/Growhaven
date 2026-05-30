#include <iostream>
#include "entity/Player.hpp"
#include "core/KeyHandler.hpp"
#include <cmath>

Player::Player(
    const GameConfig &config, 
    const KeyHandler& keyHandler
) : 
    Entity(config),
    keyHandler(keyHandler)
{
    setDefaultValues();
};


void Player::setDefaultValues()
{
    // SET PLAYER SPAWN POINT
    worldX = 13;
    worldY = 9;

    speed = 300.0f;
    direction = Direction::Down;
    animationTimer = 0;


    if(!texture1.loadFromFile("res/player/square_man_1.png"))
    {
        throw std::runtime_error("Failed to load texture: square_man_1");
    }
    if (!texture2.loadFromFile("res/player/square_man_2.png"))
    {
        throw std::runtime_error("Failed to load texture: square_man_2");
    }

    spriteNum = 1;
}

void Player::update(float deltaTime)
{
    // MOVEMENTS
    if (keyHandler.upPressed)
    {
        worldY -= (speed * deltaTime) / config.tileSize;
    }
    if (keyHandler.downPressed)
    {
        worldY += (speed * deltaTime) / config.tileSize;
    }
    if (keyHandler.leftPressed)
    {
        worldX -= (speed * deltaTime) / config.tileSize;
    }
    if (keyHandler.rightPressed)
    {
        worldX += (speed * deltaTime) / config.tileSize;
    }

    // ANIMATION
    animationTimer += deltaTime;

    if (animationTimer >= 0.15f)
    {
        if (spriteNum == 1)
        {
            spriteNum = 2;
        }

        else if (spriteNum == 2)
        {
            spriteNum = 1;
        }

        animationTimer -= 0.15f;
    }


}

void Player::render(sf::RenderWindow &window)
{
    sf::Sprite sprite(spriteNum == 1 ? texture1 : texture2);
    sf::Vector2f pos(
        worldX * config.tileSize - config.tileSize,
        worldY * config.tileSize - config.tileSize
    );
    sf::Vector2f scale(
        config.scale,
        config.scale);

    sprite.setPosition(pos);
    sprite.setScale(scale);

    window.draw(sprite);
}
