#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include "../Button/button.cpp"

class Menu
{
private:
    sf::RectangleShape background;
    sf::Font font;
    std::optional<sf::Text> titleText;

    std::vector<std::unique_ptr<Button>> buttons;

public:
    Menu(float windowWidth, float windowHeight);

    bool loadFont(const std::string &fontPath);
    Button *addButton(const std::string &text, float y);

    void handleMouseMove(sf::Vector2f mousePos);
    void handleMousePress(sf::Vector2f mousePos);
    void handleMouseRelease();

    void draw(sf::RenderWindow &window);
};
