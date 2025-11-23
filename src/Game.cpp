#include "Game.hpp"
#include <iostream>
#include <optional>
#include <algorithm>

Game::Game()
    : window(sf::VideoMode({Paths::WINDOW_WIDTH, Paths::WINDOW_HEIGHT}), "I am not a hero"),
      gameState(GameState::Menu),
      menu(static_cast<float>(Paths::WINDOW_WIDTH), static_cast<float>(Paths::WINDOW_HEIGHT)),
      ground(Paths::GROUND_TILESET_TEXTURE, 32, 32),
      player(Paths::PLAYER_IDLE_TEXTURE, Paths::PLAYER_RUN_TEXTURE, Paths::PLAYER_JUMP_TEXTURE, Paths::PLAYER_ATTACK_TEXTURE, 100.f, 100.f),
      camera(sf::FloatRect({0.f, 0.f}, {800.f, 600.f})),
      cameraSmoothing(0.1f),
      mapWidth(1000),
      mapHeight(1000),
      tileSizeX(32),
      tileSizeY(32)
{
    window.setFramerateLimit(60);

    if (!menu.loadFont(Paths::FONT_PATH))
    {
        std::cerr << "Failed to load font for menu!" << std::endl;
    }

    // Buttons
    Button *playButton = menu.addButton("PLAY", 300.f);
    playButton->setOnClick([this]()
                           {
        gameState = GameState::Playing;
        std::cout << "Starting game..." << std::endl; });

    Button *optionsButton = menu.addButton("OPTIONS", 380.f);
    optionsButton->setOnClick([]()
                              { std::cout << "Options clicked!" << std::endl; });

    Button *exitButton = menu.addButton("EXIT", 460.f);
    exitButton->setOnClick([this]()
                           { window.close(); });

    // Platforms (example layout)
    ground.createHorizontalPlatform(0, mapHeight, 35, 1, 0); // ground bottom
    ground.createHorizontalPlatform(0, 0, 35, 1, 0);         // wall top
    ground.createVerticalPlatform(0, 0, 35, 2, 1);           // wall left
    ground.createVerticalPlatform(mapWidth, 0, 35, 0, 1);    // wall right
    ground.createHorizontalPlatform(200, 400, 8, 1, 0);      // platform middle
    ground.createHorizontalPlatform(50, 300, 5, 1, 0);       // platform left top
    ground.createHorizontalPlatform(500, 250, 6, 1, 0);      // platform right top
}

int Game::run()
{
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(mousePixelPos);

        processEvents(mousePos);
        update(deltaTime, mousePos);
        render();
    }

    return 0;
}

void Game::processEvents(const sf::Vector2f &mousePos)
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }

        if (gameState == GameState::Menu)
        {
            if (const auto *mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mousePressed->button == sf::Mouse::Button::Left)
                {
                    menu.handleMousePress(mousePos);
                }
            }

            if (const auto *mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouseReleased->button == sf::Mouse::Button::Left)
                {
                    menu.handleMouseRelease();
                }
            }
        }
    }
}

void Game::update(float deltaTime, const sf::Vector2f &mousePos)
{
    if (gameState == GameState::Menu)
    {
        menu.handleMouseMove(mousePos);
    }
    else if (gameState == GameState::Playing)
    {
        player.handleInput();
        player.update(deltaTime, ground.getCollisionBoxes());

        // CAMERA FOLLOW PLAYER
        sf::Vector2f targetCameraPos = player.getPosition();

        float lookAheadDistance = 25.f;
        if (!player.isFacingRight())
        {
            targetCameraPos.x -= lookAheadDistance;
        }
        else
        {
            targetCameraPos.x += lookAheadDistance;
        }

        sf::Vector2f currentCameraPos = camera.getCenter();
        sf::Vector2f newCameraPos = currentCameraPos + (targetCameraPos - currentCameraPos) * cameraSmoothing;
        camera.setCenter(newCameraPos);

        // Limit camera within map bounds
        float halfWidth = camera.getSize().x / 2.f;
        float halfHeight = camera.getSize().y / 2.f;

        float minX = halfWidth;
        float maxX = static_cast<float>(mapWidth) - halfWidth + static_cast<float>(tileSizeX);
        float minY = halfHeight;
        float maxY = static_cast<float>(mapHeight) - halfHeight + static_cast<float>(tileSizeY);

        sf::Vector2f boundedPos = camera.getCenter();
        boundedPos.x = std::max(minX, std::min(boundedPos.x, maxX));
        boundedPos.y = std::max(minY, std::min(boundedPos.y, maxY));

        camera.setCenter(boundedPos);

        window.setView(camera);
    }
}

void Game::render()
{
    window.clear(sf::Color(135, 206, 235));

    if (gameState == GameState::Menu)
    {
        menu.draw(window);
    }
    else if (gameState == GameState::Playing)
    {
        ground.draw(window);
        player.draw(window);
        player.drawAttackHitbox(window);

        // Debug player collision hitbox
        sf::RectangleShape debugHitbox;
        debugHitbox.setFillColor(sf::Color::Transparent);
        debugHitbox.setOutlineColor(sf::Color::Green);
        debugHitbox.setOutlineThickness(1.f);

        sf::FloatRect collisionRect = player.getCollisionHitbox();
        debugHitbox.setPosition(collisionRect.position);
        debugHitbox.setSize(collisionRect.size);

        window.draw(debugHitbox);
    }

    window.display();
}
