#include "Button.hpp"
#include <iostream>

Button::Button(float x, float y, float width, float height)
    : shape({width, height}),
      font(nullptr),
      // 'std::optional<sf::Text> text' akan otomatis
      // dibuat sebagai 'kosong' (std::nullopt)
      normalColor(sf::Color(70, 70, 100)),
      hoverColor(sf::Color(100, 100, 150)),
      pressedColor(sf::Color(50, 50, 80)),
      textNormalColor(sf::Color::White),
      textHoverColor(sf::Color::Yellow),
      isHovered(false),
      isPressed(false)
{
    // Setup shape
    shape.setPosition({x, y});
    shape.setFillColor(normalColor);
    shape.setOutlineThickness(3.f);
    shape.setOutlineColor(sf::Color::White);

    // HAPUS 'text.setCharacterSize' dan 'text.setFillColor'
    // karena 'text' belum ada (masih kosong)
}

void Button::setFont(sf::Font &buttonFont)
{
    font = &buttonFont;

    // ===== PERBAIKAN: Buat (em-place) objek sf::Text SEKARANG =====
    // Kita membuat 'text' di sini, saat kita akhirnya punya FONT.
    text.emplace("", *font, 24);         // 'emplace' membuat objek di dalam optional
    text->setFillColor(textNormalColor); // Gunakan -> untuk akses
}

void Button::setText(const std::string &buttonText)
{
    // Cek jika 'text' sudah dibuat (setFont sudah dipanggil)
    if (text)
    {
        text->setString(buttonText);

        // Center text in button
        sf::FloatRect textBounds = text->getLocalBounds();
        sf::FloatRect shapeBounds = shape.getGlobalBounds();

        text->setOrigin({textBounds.position.x + textBounds.size.x / 2.f,
                         textBounds.position.y + textBounds.size.y / 2.f});
        text->setPosition({shapeBounds.position.x + shapeBounds.size.x / 2.f,
                           shapeBounds.position.y + shapeBounds.size.y / 2.f});
    }
}

void Button::setTextSize(unsigned int size)
{
    if (text)
    {
        text->setCharacterSize(size);
        setText(text->getString());
    }
}

void Button::setPosition(float x, float y)
{
    shape.setPosition({x, y});
    // Update text position (pengecekan 'if (text)' sudah ada di dalam setText)
    setText(text ? text->getString() : ""); // Panggil ulang setText
}

void Button::setSize(float width, float height)
{
    shape.setSize({width, height});
    // Update text position (pengecekan 'if (text)' sudah ada di dalam setText)
    setText(text ? text->getString() : ""); // Panggil ulang setText
}

void Button::setColors(sf::Color normal, sf::Color hover, sf::Color pressed)
{
    normalColor = normal;
    hoverColor = hover;
    pressedColor = pressed;
    if (!isHovered && !isPressed)
        shape.setFillColor(normalColor);
}

void Button::setTextColors(sf::Color normal, sf::Color hover)
{
    textNormalColor = normal;
    textHoverColor = hover;
    if (text && !isHovered)                  // Cek
        text->setFillColor(textNormalColor); // Gunakan ->
}

void Button::setOnClick(std::function<void()> callback)
{
    onClick = callback;
}

void Button::handleMouseMove(sf::Vector2f mousePos)
{
    bool wasHovered = isHovered;
    isHovered = isMouseOver(mousePos);

    if (isHovered && !wasHovered)

    {
        if (!isPressed)

        {
            shape.setFillColor(hoverColor);
            if (text)
                text->setFillColor(textHoverColor); // Cek & Gunakan ->
        }
    }
    else if (!isHovered && wasHovered) // Baru keluar

    {
        if (!isPressed)

        {
            shape.setFillColor(normalColor);
            if (text)
                text->setFillColor(textNormalColor); // Cek & Gunakan ->
        }
    }
}

bool Button::handleMousePress(sf::Vector2f mousePos)
{
    if (isMouseOver(mousePos))

    {
        isPressed = true;
        shape.setFillColor(pressedColor);
        return true;
    }
    return false;
}

void Button::handleMouseRelease()
{
    if (isPressed)

    {
        isPressed = false;

        if (isHovered) // Dilepas di dalam

        {
            if (onClick)

            {
                onClick();
            }
            shape.setFillColor(hoverColor);
            if (text)
                text->setFillColor(textHoverColor); // Cek & Gunakan ->
        }
        else // Dilepas di luar

        {
            shape.setFillColor(normalColor);
            if (text)
                text->setFillColor(textNormalColor); // Cek & Gunakan ->
        }
    }
}

bool Button::isMouseOver(sf::Vector2f mousePos) const
{
    return shape.getGlobalBounds().contains(mousePos);
}

bool Button::getIsHovered() const
{
    return isHovered;
}

sf::FloatRect Button::getBounds() const
{
    return shape.getGlobalBounds();
}

void Button::draw(sf::RenderWindow &window)
{
    window.draw(shape);

    // Hanya gambar teks jika 'text' sudah dibuat
    if (text)

    {
        window.draw(*text); // Gunakan * untuk dereference saat menggambar
    }
}