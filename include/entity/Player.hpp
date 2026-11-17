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

    // LAST DIRECTION THE PLAYER WALKED IN (NEVER None), USED FOR SPRITE SELECTION
    Direction facing;

    // WALK FRAMES INDEXED AS [Up, Down, Left, Right][frame 1, frame 2]
    sf::Texture walkTextures[4][2];

    const KeyHandler &keyHandler;
    void setDefaultValues();
    void loadTextures();

};