#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include "components/Button.hpp"
#include "BaseScene.hpp"
#include "GameState.hpp"

class MenuScene : public BaseScene
{
private:
    sf::RectangleShape m_background;
    sf::Font m_font;
    std::optional<sf::Text> m_titleText;

    std::vector<std::unique_ptr<Button>> m_buttons;

    GameState m_nextState;

public:
    MenuScene(float windowWidth, float windowHeight);
    virtual ~MenuScene() = default;

    void handleEvent(const sf::Event &event, sf::RenderWindow &window) override;
    GameState update(float deltaTime) override;

    void draw(sf::RenderWindow &window) override;

    bool loadFont(const std::string &fontPath);
    Button *addButton(const std::string &text, float y);

    void handleMouseMove(sf::Vector2f mousePos);
    void handleMousePress(sf::Vector2f mousePos);
    void handleMouseRelease();
};