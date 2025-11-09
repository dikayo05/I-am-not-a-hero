#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include <optional>

class Button
{
private:
    sf::RectangleShape shape;
    sf::Font *font;
    std::optional<sf::Text> text;

    sf::Color normalColor;
    sf::Color hoverColor;
    sf::Color pressedColor;
    sf::Color textNormalColor;
    sf::Color textHoverColor;

    bool isHovered;
    bool isPressed;

    std::function<void()> onClick;

public:
    // Constructor
    Button(float x, float y, float width, float height);

    // Setup
    void setFont(sf::Font &buttonFont);
    void setText(const std::string &buttonText);
    void setTextSize(unsigned int size);
    void setPosition(float x, float y);
    void setSize(float width, float height);

    // Colors
    void setColors(sf::Color normal, sf::Color hover, sf::Color pressed);
    void setTextColors(sf::Color normal, sf::Color hover);

    // Callback
    void setOnClick(std::function<void()> callback);

    // Interaction
    void handleMouseMove(sf::Vector2f mousePos);
    bool handleMousePress(sf::Vector2f mousePos);
    void handleMouseRelease();

    // Getters
    bool isMouseOver(sf::Vector2f mousePos) const;
    bool getIsHovered() const;
    sf::FloatRect getBounds() const;

    // Draw
    void draw(sf::RenderWindow &window);
};
