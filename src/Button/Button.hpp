#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include <optional>

class Button
{
private:
    sf::RectangleShape m_shape;
    sf::Font *m_font;
    std::optional<sf::Text> m_text;

    sf::Color m_normalColor;
    sf::Color m_hoverColor;
    sf::Color m_pressedColor;
    sf::Color m_textNormalColor;
    sf::Color m_textHoverColor;

    bool m_isHovered;
    bool m_isPressed;

    std::function<void()> m_onClick;

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
