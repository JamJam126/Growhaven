#include <iostream>
#include "core/KeyHandler.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"

void KeyHandler::handleKeyEvent(const sf::Event &event)
{
    if (event.is<sf::Event::KeyPressed>()) 
    {
        auto key = event.getIf<sf::Event::KeyPressed>()->code;

        if (key == sf::Keyboard::Key::W) 
        {
            upPressed = true;
        }
        if (key == sf::Keyboard::Key::A)
        {
            leftPressed = true;
        }
        if (key == sf::Keyboard::Key::S)
        {
            downPressed = true;
        }
        if (key == sf::Keyboard::Key::D)
        {
            rightPressed = true;
        }
    }

    if (event.is<sf::Event::KeyReleased>()) 
    {
        auto key = event.getIf<sf::Event::KeyReleased>()->code;

        if (key == sf::Keyboard::Key::W)
        {
            upPressed = false;
        }
        else if (key == sf::Keyboard::Key::A)
        {
            leftPressed = false;
        }
        else if (key == sf::Keyboard::Key::S)
        {
            downPressed = false;
        }
        else if (key == sf::Keyboard::Key::D)
        {
            rightPressed = false;
        }
    }
}