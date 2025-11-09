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

    // Buat ground dengan tileset
    Ground ground(GROUND_TILESET_TEXTURE, 32, 32);

    // Buat beberapa platform
    // Platform bawah (ground utama)
    ground.createPlatform(0, 500, 25, 1, 0); // x, y, panjang, tileX, tileY

    // Platform tengah
    ground.createPlatform(200, 400, 8, 1, 0);

    // Platform kiri atas
    ground.createPlatform(50, 300, 5, 1, 0);

    // Platform kanan atas
    ground.createPlatform(500, 250, 6, 1, 0);

    // Buat player
    Player player(PLAYER_IDLE_TEXTURE, PLAYER_RUN_TEXTURE, PLAYER_JUMP_TEXTURE, 100.f, 100.f);

    // Opsional: Sesuaikan physics
    // player.setJumpForce(500.f);
    // player.setGravity(1200.f);

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

                // Reset player position (R key)
                if (keyPressed->code == sf::Keyboard::Key::R)
                {
                    player = Player(PLAYER_IDLE_TEXTURE, PLAYER_RUN_TEXTURE, PLAYER_JUMP_TEXTURE, 100.f, 100.f);
                }
            }
        }

        // Update
        player.handleInput();
        player.update(deltaTime, ground.getCollisionBoxes());

        // Render
        window.clear(sf::Color(135, 206, 235)); // Sky blue
        ground.draw(window);
        player.draw(window);
        window.display();
    }

    return 0;
}