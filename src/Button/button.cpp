#include "Button.hpp"
#include <iostream>

Button::Button(float x, float y, float width, float height)
    : m_shape({width, height}),
      m_font(nullptr),
      m_normalColor(sf::Color(70, 70, 100)),
      m_hoverColor(sf::Color(100, 100, 150)),
      m_pressedColor(sf::Color(50, 50, 80)),
      m_textNormalColor(sf::Color::White),
      m_textHoverColor(sf::Color::Yellow),
      m_isHovered(false),
      m_isPressed(false)
{
    // Setup shape
    m_shape.setPosition({x, y});
    m_shape.setFillColor(m_normalColor);
    m_shape.setOutlineThickness(3.f);
    m_shape.setOutlineColor(sf::Color::White);
}

void Button::setFont(sf::Font &buttonFont)
{
    m_font = &buttonFont;

    // make sure to create the text object, because using std::optional
    m_text.emplace(*m_font, "", 24); // 'emplace' make the object in place
    m_text->setFillColor(m_textNormalColor);
}

void Button::setText(const std::string &buttonText)
{
    // Only set text if m_text has been initialized
    if (m_text)
    {
        m_text->setString(buttonText);

        // Center text in button
        sf::FloatRect textBounds = m_text->getLocalBounds();
        sf::FloatRect shapeBounds = m_shape.getGlobalBounds();

        m_text->setOrigin({textBounds.position.x + textBounds.size.x / 2.f,
                           textBounds.position.y + textBounds.size.y / 2.f});
        m_text->setPosition({shapeBounds.position.x + shapeBounds.size.x / 2.f,
                             shapeBounds.position.y + shapeBounds.size.y / 2.f});
    }
}

void Button::setTextSize(unsigned int size)
{
    if (m_text)
    {
        m_text->setCharacterSize(size);
        setText(m_text->getString());
    }
}

void Button::setPosition(float x, float y)
{
    m_shape.setPosition({x, y});
    // Update text position (check 'if (text)' already in setText)
    setText(m_text ? m_text->getString() : ""); // Call setText again
}

void Button::setSize(float width, float height)
{
    m_shape.setSize({width, height});
    // Update text position (check 'if (text)' already in setText)
    setText(m_text ? m_text->getString() : ""); // Call setText again
}

void Button::setColors(sf::Color normal, sf::Color hover, sf::Color pressed)
{
    m_normalColor = normal;
    m_hoverColor = hover;
    m_pressedColor = pressed;
    if (!m_isHovered && !m_isPressed)
        m_shape.setFillColor(m_normalColor);
}

void Button::setTextColors(sf::Color normal, sf::Color hover)
{
    m_textNormalColor = normal;
    m_textHoverColor = hover;
    if (m_text && !m_isHovered)
        m_text->setFillColor(m_textNormalColor);
}

void Button::setOnClick(std::function<void()> callback)
{
    m_onClick = callback;
}

void Button::handleMouseMove(sf::Vector2f mousePos)
{
    bool wasHovered = m_isHovered;
    m_isHovered = isMouseOver(mousePos);

    if (m_isHovered && !wasHovered)
    {
        if (!m_isPressed)
        {
            m_shape.setFillColor(m_hoverColor);
            if (m_text)
                m_text->setFillColor(m_textHoverColor);
        }
    }
    else if (!m_isHovered && wasHovered)
    {
        if (!m_isPressed)
        {
            m_shape.setFillColor(m_normalColor);
            if (m_text)
                m_text->setFillColor(m_textNormalColor);
        }
    }
}

bool Button::handleMousePress(sf::Vector2f mousePos)
{
    if (isMouseOver(mousePos))
    {
        m_isPressed = true;
        m_shape.setFillColor(m_pressedColor);
        return true;
    }
    return false;
}

void Button::handleMouseRelease()
{
    if (m_isPressed)
    {
        m_isPressed = false;

        if (m_isHovered) // released while still hovering
        {
            if (m_onClick)
            {
                m_onClick();
            }
            m_shape.setFillColor(m_hoverColor);
            if (m_text)
            {
                m_text->setFillColor(m_textHoverColor);
            }
        }
        else // released outside
        {
            m_shape.setFillColor(m_normalColor);
            if (m_text)
            {
                m_text->setFillColor(m_textNormalColor);
            }
        }
    }
}

bool Button::isMouseOver(sf::Vector2f mousePos) const
{
    return m_shape.getGlobalBounds().contains(mousePos);
}

bool Button::getIsHovered() const
{
    return m_isHovered;
}

sf::FloatRect Button::getBounds() const
{
    return m_shape.getGlobalBounds();
}

void Button::draw(sf::RenderWindow &window)
{
    window.draw(m_shape);

    // only draw text if it exists
    if (m_text)
    {
        window.draw(*m_text);
    }
}