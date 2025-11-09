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
    float cameraSmoothing = 0.1f; // Semakin kecil = semakin smooth (0.05f - 1.0f)

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

        // CAMERA FOLLOW PLAYER
        sf::Vector2f targetCameraPos = player.getPosition();

        // Camera look-ahead berdasarkan arah player
        float lookAheadDistance = 25.f; // Jarak look-ahead
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

        // Batasi camera agar tidak keluar dari batas map (opsional)
        // float halfWidth = camera.getSize().x / 2.f;
        // float halfHeight = camera.getSize().y / 2.f;

        // Set batas map Anda (sesuaikan dengan ukuran level)
        // float minX = halfWidth;
        // float maxX = 2000.f - halfWidth; // Misal lebar map 2000
        // float minY = halfHeight;
        // float maxY = 1000.f - halfHeight; // Misal tinggi map 1000

        // sf::Vector2f boundedPos = camera.getCenter();
        // boundedPos.x = std::max(minX, std::min(boundedPos.x, maxX));
        // boundedPos.y = std::max(minY, std::min(boundedPos.y, maxY));

        // camera.setCenter(boundedPos);

        window.setView(camera);

        // RENDER
        window.clear(sf::Color(135, 206, 235));
        ground.draw(window);
        player.draw(window);

        // Reset view ke default untuk UI/HUD (jika ada)
        // window.setView(window.getDefaultView());

        window.display();
    }

    return 0;
}