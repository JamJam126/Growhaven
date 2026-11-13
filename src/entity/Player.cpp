#include <iostream>
#include "entity/Player.hpp"
#include "core/KeyHandler.hpp"
#include <cmath>

Player::Player(
    const GameConfig& config, 
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
    worldX = 12 * config.tileSize;
    worldY = 8  * config.tileSize;

    speed = 300.0f;
    direction = Direction::Up;
    isMoving = false;
    animationTimer = 0;


    if(!texture1.loadFromFile("res/player/farmer_down_1.png"))
    {
        throw std::runtime_error("Failed to load texture: farmer_down_1");
    }
    if (!texture2.loadFromFile("res/player/farmer_down_2.png"))
    {
        throw std::runtime_error("Failed to load texture: farmer_down_2");
    }

    spriteNum = 1;
}

void Player::updateDirection()
{
    isMoving = true;

    if (keyHandler.upPressed)
    {
        direction = Direction::Up;
    }
    else if (keyHandler.downPressed)
    {
        direction = Direction::Down;
    }
    else if (keyHandler.leftPressed)
    {
        direction = Direction::Left;
    }
    else if (keyHandler.rightPressed)
    {
        direction = Direction::Right;
    }
    else 
    {
        direction = Direction::None;
        isMoving = false;
    }
}

void Player::update(float deltaTime)
{
    // PLAYER CAN MOVE IF THERE IS NO COLLISION
    if (collisionOn == false && isMoving == true)
    {
        switch (direction)
        {
        case Direction::Up:
            worldY -= (speed * deltaTime);
            break;

        case Direction::Down:
            worldY += (speed * deltaTime);
            break;
        
        case Direction::Left:
            worldX -= (speed * deltaTime);
            break;
        
        case Direction::Right:
            worldX += (speed * deltaTime);
            break;
        }
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
        worldX,
        worldY
    );
    sf::Vector2f scale(
        config.scale,
        config.scale);

    sprite.setPosition(pos);
    sprite.setScale(scale);

    window.draw(sprite);

    // DEBUGGING
    // sf::RectangleShape debugBox;
    // debugBox.setPosition(sf::Vector2f(worldX, worldY));
    // debugBox.setSize(sf::Vector2f(config.tileSize, config.tileSize));
    // debugBox.setFillColor(sf::Color::Transparent);
    // debugBox.setOutlineColor(sf::Color::Red);
    // debugBox.setOutlineThickness(1.0f);

    // window.draw(debugBox);
}
