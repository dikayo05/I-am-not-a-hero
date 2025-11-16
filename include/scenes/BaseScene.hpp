#pragma once

#include <SFML/Graphics.hpp>
#include "GameState.hpp"

class BaseScene
{
public:
    virtual ~BaseScene() = default;

    virtual void handleEvent(const sf::Event &event, sf::RenderWindow &window) = 0;

    virtual GameState update(float deltaTime) = 0;

    virtual void draw(sf::RenderWindow &window) = 0;
};