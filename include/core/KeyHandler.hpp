#pragma once
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

class KeyHandler {
public:
    bool upPressed = false;
    bool downPressed = false;
    bool leftPressed = false;
    bool rightPressed = false;

    // DEBUG (toggled with F3)
    bool showDebug = false;

    void handleKeyEvent(const sf::Event &event);

private:
    bool f3Held = false; // IGNORE KEY REPEAT WHILE F3 IS HELD
};