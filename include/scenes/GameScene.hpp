#pragma once
#include "BaseScene.hpp"
#include "components/Player.hpp"
#include "components/Ground.hpp"
#include "Constants.hpp"

class GameScene : public BaseScene
{
public:
    GameScene();
    virtual ~GameScene() = default;

    void handleEvent(const sf::Event &event, sf::RenderWindow &window) override;
    GameState update(float deltaTime) override;
    void draw(sf::RenderWindow &window) override;

private:
    void initLevel();
    void updateCamera(float deltaTime);
    void drawDebug(sf::RenderWindow &window);

    Player m_player;
    Ground m_ground;
    sf::View m_camera;
    GameState m_nextState;

    int m_mapWidth;
    int m_mapHeight;
    int m_tileSizeX;
    int m_tileSizeY;
    float m_cameraSmoothing;
};