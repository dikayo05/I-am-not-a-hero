#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Constants.hpp"
#include "components/Player.hpp"
#include "components/Ground.hpp"
#include "scenes/MenuScene.hpp"

enum class GameState
{
    Menu,
    Playing
};

int main()
{
    sf::RenderWindow window(sf::VideoMode({Paths::WINDOW_WIDTH, Paths::WINDOW_HEIGHT}), "I am not a hero");
    window.setFramerateLimit(60);

    GameState gameState = GameState::Menu;

    // Menu
    Menu menu(static_cast<float>(Paths::WINDOW_WIDTH), static_cast<float>(Paths::WINDOW_HEIGHT));
    if (!menu.loadFont(Paths::FONT_PATH))
    {
        std::cerr << "Failed to load font for menu!" << std::endl;
    }

    // Add buttons with callback
    Button *playButton = menu.addButton("PLAY", 300.f);
    playButton->setOnClick([&gameState]()
                           {
        gameState = GameState::Playing;
        std::cout << "Starting game..." << std::endl; });

    Button *optionsButton = menu.addButton("OPTIONS", 380.f);
    optionsButton->setOnClick([]()
                              { std::cout << "Options clicked!" << std::endl; });

    Button *exitButton = menu.addButton("EXIT", 460.f);
    exitButton->setOnClick([&window]()
                           { window.close(); });

    // PLATFORMS
    Ground ground(Paths::GROUND_TILESET_TEXTURE, 32, 32);
    // x, y, long, tileX, tileY
    ground.createPlatform(0, 500, 25, 1, 0);  // Platform down (main ground)
    ground.createPlatform(200, 400, 8, 1, 0); // Platform middle
    ground.createPlatform(50, 300, 5, 1, 0);  // Platform left top
    ground.createPlatform(500, 250, 6, 1, 0); // Platform right top

    // PLAYER
    Player player(Paths::PLAYER_IDLE_TEXTURE, Paths::PLAYER_RUN_TEXTURE, Paths::PLAYER_JUMP_TEXTURE, Paths::PLAYER_ATTACK_TEXTURE, 100.f, 100.f);

    // CAMERA
    sf::View camera(sf::FloatRect({0.f, 0.f}, {800.f, 600.f}));
    float cameraSmoothing = 0.1f; // the smaller = the finer (0.05f - 1.0f)

    sf::Clock clock;

    // Game loop
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(mousePixelPos);

        // Handle events
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

        // UPDATE
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

            // Camera look-ahead based on player facing direction
            float lookAheadDistance = 25.f; // look-ahead distance
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

            // Limit the camera so it doesn't go beyond the map boundaries (optional)
            float halfWidth = camera.getSize().x / 2.f;
            float halfHeight = camera.getSize().y / 2.f;

            // Set your map boundaries (adjust according to the level size)
            float minX = halfWidth;
            float maxX = 2000.f - halfWidth; // For example, the map width is 2000
            float minY = halfHeight;
            float maxY = 1000.f - halfHeight; // For example, map height 1000

            sf::Vector2f boundedPos = camera.getCenter();
            boundedPos.x = std::max(minX, std::min(boundedPos.x, maxX));
            boundedPos.y = std::max(minY, std::min(boundedPos.y, maxY));

            camera.setCenter(boundedPos);
            // end limit the camera

            window.setView(camera);
        }

        // RENDER
        window.clear(sf::Color(135, 206, 235));

        if (gameState == GameState::Menu)
        {
            menu.draw(window);
        }
        else if (gameState == GameState::Playing)
        {
            ground.draw(window);
            player.draw(window);
            player.drawAttackHitbox(window); // Attack hitbox image for debugging

            // Reset view to default for UI/HUD (if any)
            // window.setView(window.getDefaultView());

            // Player collision hitbox image for debugging
            sf::RectangleShape debugHitbox;
            debugHitbox.setFillColor(sf::Color::Transparent);
            debugHitbox.setOutlineColor(sf::Color::Green);
            debugHitbox.setOutlineThickness(1.f);

            sf::FloatRect collisionRect = player.getCollisionHitbox();

            debugHitbox.setPosition(collisionRect.position);
            debugHitbox.setSize(collisionRect.size);

            window.draw(debugHitbox);
            // end player collision hitbox
        }

        window.display();
    }

    return 0;
}