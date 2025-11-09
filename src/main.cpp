#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Player/player.cpp"
#include "Ground/ground.cpp"
#include "Scenes/main-menu.cpp"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define ASSET_PATH "../../assets/"

// FONT
#define FONT_PATH ASSET_PATH "fonts/Lexend/static/Lexend-Regular.ttf"

// PLAYER TEXTURES
#define PLAYER_IDLE_TEXTURE ASSET_PATH "images/character/player/knight/Idle.png"
#define PLAYER_RUN_TEXTURE ASSET_PATH "images/character/player/knight/Run.png"
#define PLAYER_JUMP_TEXTURE ASSET_PATH "images/character/player/knight/Jump.png"
#define PLAYER_ATTACK_TEXTURE ASSET_PATH "images/character/player/knight/Attacks.png"

// GROUND TILESET TEXTURE
#define GROUND_TILESET_TEXTURE ASSET_PATH "images/tilesets/tx_tileset_ground.png"

enum class GameState
{
    Menu,
    Playing
};

int main()
{
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "I am not a hero");
    window.setFramerateLimit(60);

    GameState gameState = GameState::Menu;

    // Menu
    Menu menu(static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT));
    if (!menu.loadFont(FONT_PATH))
    {
        std::cerr << "Failed to load font for menu!" << std::endl;
    }

    // Add buttons dengan callback
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
    Ground ground(GROUND_TILESET_TEXTURE, 32, 32);
    // x, y, panjang, tileX, tileY
    ground.createPlatform(0, 500, 25, 1, 0);  // Platform bawah (ground utama)
    ground.createPlatform(200, 400, 8, 1, 0); // Platform tengah
    ground.createPlatform(50, 300, 5, 1, 0);  // Platform kiri atas
    ground.createPlatform(500, 250, 6, 1, 0); // Platform kanan atas

    // PLAYER
    Player player(PLAYER_IDLE_TEXTURE, PLAYER_RUN_TEXTURE, PLAYER_JUMP_TEXTURE, PLAYER_ATTACK_TEXTURE, 100.f, 100.f);

    // CAMERA
    sf::View camera(sf::FloatRect({0.f, 0.f}, {800.f, 600.f}));
    float cameraSmoothing = 0.1f; // Semakin kecil = semakin smooth (0.05f - 1.0f)

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
            float halfWidth = camera.getSize().x / 2.f;
            float halfHeight = camera.getSize().y / 2.f;

            // Set batas map Anda (sesuaikan dengan ukuran level)
            float minX = halfWidth;
            float maxX = 2000.f - halfWidth; // Misal lebar map 2000
            float minY = halfHeight;
            float maxY = 1000.f - halfHeight; // Misal tinggi map 1000

            sf::Vector2f boundedPos = camera.getCenter();
            boundedPos.x = std::max(minX, std::min(boundedPos.x, maxX));
            boundedPos.y = std::max(minY, std::min(boundedPos.y, maxY));

            camera.setCenter(boundedPos);
            // end batasi

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
            player.drawAttackHitbox(window); // Gambar hitbox serangan untuk debugging

            // Reset view ke default untuk UI/HUD (jika ada)
            // window.setView(window.getDefaultView());

            // Gambar hitbox collision player untuk debugging
            sf::RectangleShape debugHitbox;
            debugHitbox.setFillColor(sf::Color::Transparent);
            debugHitbox.setOutlineColor(sf::Color::Green);
            debugHitbox.setOutlineThickness(1.f);

            sf::FloatRect collisionRect = player.getCollisionHitbox();

            debugHitbox.setPosition(collisionRect.position);
            debugHitbox.setSize(collisionRect.size);

            window.draw(debugHitbox);
            // end Gambar hitbox
        }

        window.display();
    }

    return 0;
}