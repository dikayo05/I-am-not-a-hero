#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Player/player.cpp"
#include "Ground/ground.cpp"

#define ASSET_PATH "../../assets/"

// PLAYER TEXTURES
#define PLAYER_IDLE_TEXTURE ASSET_PATH "images/character/player/knight/Idle.png"
#define PLAYER_RUN_TEXTURE ASSET_PATH "images/character/player/knight/Run.png"
#define PLAYER_JUMP_TEXTURE ASSET_PATH "images/character/player/knight/Jump.png"

// GROUND TILESET TEXTURE
#define GROUND_TILESET_TEXTURE ASSET_PATH "images/tilesets/tx_tileset_ground.png"

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "I am not a hero");
    window.setFramerateLimit(60);

    // PLATFORMS
    Ground ground(GROUND_TILESET_TEXTURE, 32, 32);
    // x, y, panjang, tileX, tileY
    ground.createPlatform(0, 500, 25, 1, 0);  // Platform bawah (ground utama)
    ground.createPlatform(200, 400, 8, 1, 0); // Platform tengah
    ground.createPlatform(50, 300, 5, 1, 0);  // Platform kiri atas
    ground.createPlatform(500, 250, 6, 1, 0); // Platform kanan atas

    // PLAYER
    Player player(PLAYER_IDLE_TEXTURE, PLAYER_RUN_TEXTURE, PLAYER_JUMP_TEXTURE, 100.f, 100.f);
    // Opsional: Sesuaikan physics
    // player.setJumpForce(500.f);
    // player.setGravity(1200.f);

    // CAMERA
    sf::View camera(sf::FloatRect({0.f, 0.f}, {800.f, 600.f}));
    float cameraSmoothing = 0.1f; // Semakin kecil = semakin smooth (0.05-0.2)

    sf::Clock clock;

    // Game loop
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        // Handle events
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::Escape)
                {
                    window.close();
                }
            }
        }

        // UPDATE
        player.handleInput();
        player.update(deltaTime, ground.getCollisionBoxes());

        // Smooth camera follow player
        sf::Vector2f targetCameraPos = player.getPosition();
        sf::Vector2f currentCameraPos = camera.getCenter();

        // Lerp (linear interpolation) untuk smooth movement
        sf::Vector2f newCameraPos = currentCameraPos + (targetCameraPos - currentCameraPos) * cameraSmoothing;

        camera.setCenter(newCameraPos);
        window.setView(camera);

        // RENDER
        window.clear(sf::Color(135, 206, 235));
        ground.draw(window);
        player.draw(window);
        window.display();
    }

    return 0;
}