#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "scenes/BaseScene.hpp"
#include "scenes/GameState.hpp"

class Game
{
public:
    Game();
    void run();

private:
    void changeScene(GameState newState);
    void handleEvents();
    void update(float deltaTime);
    void render();

    sf::RenderWindow m_window;
    std::unique_ptr<BaseScene> m_currentScene;
    GameState m_currentState;
    sf::Clock m_clock;
};