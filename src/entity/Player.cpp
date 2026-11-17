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
    facing = Direction::Down;
    isMoving = false;
    animationTimer = 0;

    loadTextures();

    spriteNum = 1;
}

void Player::loadTextures()
{
    // ORDER MUST MATCH THE Direction ENUM: Up, Down, Left, Right
    const std::string names[4] = {"up", "down", "left", "right"};

    for (int dir = 0; dir < 4; ++dir)
    {
        for (int frame = 0; frame < 2; ++frame)
        {
            const std::string file = "farmer_" + names[dir] + "_" + std::to_string(frame + 1);

            if (!walkTextures[dir][frame].loadFromFile("res/player/" + file + ".png"))
            {
                throw std::runtime_error("Failed to load texture: " + file);
            }
        }
    }
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

    // KEEP FACING THE LAST WALKED DIRECTION WHEN STANDING STILL
    if (isMoving)
    {
        facing = direction;
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
    if (isMoving)
    {
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
    else
    {
        // STANDING STILL: SHOW IDLE FRAME AND RESET THE CYCLE
        spriteNum = 1;
        animationTimer = 0;
    }

}

void Player::render(sf::RenderWindow &window, bool showDebug)
{
    sf::Sprite sprite(walkTextures[static_cast<int>(facing)][spriteNum - 1]);
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
    if (showDebug)
    {
        sf::RectangleShape debugBox;
        debugBox.setPosition(sf::Vector2f(worldX, worldY));
        debugBox.setSize(sf::Vector2f(config.tileSize, config.tileSize));
        debugBox.setFillColor(sf::Color::Transparent);
        debugBox.setOutlineColor(sf::Color::Red);
        debugBox.setOutlineThickness(1.0f);

        window.draw(debugBox);
    }
}
