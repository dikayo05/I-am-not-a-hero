#include <iostream>
#include "scenes/GameScene.hpp"

GameScene::GameScene()
    : m_player(Paths::PLAYER_IDLE_TEXTURE, Paths::PLAYER_RUN_TEXTURE, Paths::PLAYER_JUMP_TEXTURE, Paths::PLAYER_ATTACK_TEXTURE, 100.f, 100.f),
      m_tileSizeX(32),
      m_tileSizeY(32),
      m_ground(Paths::GROUND_TILESET_TEXTURE, m_tileSizeX, m_tileSizeY),
      m_camera(sf::FloatRect({0.f, 0.f}, {800.f, 600.f})),
      m_nextState(GameState::Playing),
      m_mapWidth(1000),
      m_mapHeight(1000),
      m_cameraSmoothing(0.1f)
{
    initLevel();
}

void GameScene::initLevel()
{
    m_ground.createHorizontalPlatform(0, m_mapHeight, 35, 1, 0); // ground bottom
    m_ground.createHorizontalPlatform(0, 0, 35, 1, 0);           // wall top
    m_ground.createVerticalPlatform(0, 0, 35, 2, 1);             // wall left
    m_ground.createVerticalPlatform(m_mapWidth, 0, 35, 0, 1);    // wall right
    m_ground.createHorizontalPlatform(200, 400, 8, 1, 0);        // platform middle
    m_ground.createHorizontalPlatform(50, 300, 5, 1, 0);         // platform left top
    m_ground.createHorizontalPlatform(500, 250, 6, 1, 0);        // platform right top
}

void GameScene::handleEvent(const sf::Event &event, sf::RenderWindow &window)
{
    // Event handling khusus untuk 'Playing'
    if (event.is<sf::Event::Closed>())
    {
        m_nextState = GameState::Exit;
    }
    // TODO: Tambahkan event untuk 'pause' jika perlu
    // if (event.is<sf::Event::KeyPressed>() && event.getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)
    // {
    //     m_nextState = GameState::Menu;
    // }
}

GameState GameScene::update(float deltaTime)
{
    // Logika update khusus 'Playing'
    m_player.handleInput();
    m_player.update(deltaTime, m_ground.getCollisionBoxes());
    updateCamera(deltaTime);

    return m_nextState;
}

void GameScene::updateCamera(float deltaTime)
{
    sf::Vector2f targetCameraPos = m_player.getPosition();
    float lookAheadDistance = 25.f;
    if (!m_player.isFacingRight())
    {
        targetCameraPos.x -= lookAheadDistance;
    }
    else
    {
        targetCameraPos.x += lookAheadDistance;
    }

    sf::Vector2f currentCameraPos = m_camera.getCenter();
    sf::Vector2f newCameraPos = currentCameraPos + (targetCameraPos - currentCameraPos) * m_cameraSmoothing;
    m_camera.setCenter(newCameraPos);

    // Limit camera
    float halfWidth = m_camera.getSize().x / 2.f;
    float halfHeight = m_camera.getSize().y / 2.f;
    float minX = halfWidth;
    float maxX = m_mapWidth - halfWidth + m_tileSizeX;
    float minY = halfHeight;
    float maxY = m_mapHeight - halfHeight + m_tileSizeY;

    sf::Vector2f boundedPos = m_camera.getCenter();
    boundedPos.x = std::max(minX, std::min(boundedPos.x, maxX));
    boundedPos.y = std::max(minY, std::min(boundedPos.y, maxY));
    m_camera.setCenter(boundedPos);
}

void GameScene::draw(sf::RenderWindow &window)
{
    // Logika draw khusus 'Playing'
    window.setView(m_camera);
    m_ground.draw(window);
    m_player.draw(window);

    drawDebug(window);
}

void GameScene::drawDebug(sf::RenderWindow &window)
{
    // Semua kode debug digambar di sini
    m_player.drawAttackHitbox(window);

    sf::RectangleShape debugHitbox;
    debugHitbox.setFillColor(sf::Color::Transparent);
    debugHitbox.setOutlineColor(sf::Color::Green);
    debugHitbox.setOutlineThickness(1.f);
    sf::FloatRect collisionRect = m_player.getCollisionHitbox();
    debugHitbox.setPosition(collisionRect.position);
    debugHitbox.setSize(collisionRect.size);
    window.draw(debugHitbox);
}