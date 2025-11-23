#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include "Constants.hpp"
#include "components/Player.hpp"
#include "components/Ground.hpp"
#include "scenes/MenuScene.hpp"

enum class GameState
{
    Menu,
    Playing
};

class Game
{
public:
    Game();
    int run();

private:
    void processEvents(const sf::Vector2f &mousePos);
    void update(float deltaTime, const sf::Vector2f &mousePos);
    void render();

    sf::RenderWindow window;
    GameState gameState;

    // Scene / world
    Menu menu;
    Ground ground;
    Player player;

    // Camera
    sf::View camera;
    float cameraSmoothing;

    // Timing
    sf::Clock clock;

    // Map / tiles
    int mapWidth;
    int mapHeight;
    int tileSizeX;
    int tileSizeY;
};
