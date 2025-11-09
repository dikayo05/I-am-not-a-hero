#include "MainMenu.hpp"
#include <iostream>

Menu::Menu(float windowWidth, float windowHeight)
{
    // Background
    background.setSize({windowWidth, windowHeight});
    background.setFillColor(sf::Color(30, 30, 50));
}

bool Menu::loadFont(const std::string &fontPath)
{
    if (!font.openFromFile(fontPath))
    {
        std::cerr << "Error loading font!" << std::endl;
        return false;
    }

    // Setup title
    titleText.emplace(font, "I am not a hero", 80); // Buat objek sf::Text di dalam optional
    titleText->setFillColor(sf::Color::White);
    titleText->setStyle(sf::Text::Bold);

    // Center title
    sf::FloatRect titleBounds = titleText->getLocalBounds();
    titleText->setOrigin({titleBounds.size.x / 2.f, titleBounds.size.y / 2.f});
    titleText->setPosition({background.getSize().x / 2.f, 150.f});

    return true;
}

Button *Menu::addButton(const std::string &text, float y)
{
    float centerX = background.getSize().x / 2.f;
    float buttonWidth = 200.f;
    float buttonHeight = 60.f;

    auto button = std::make_unique<Button>(
        centerX - buttonWidth / 2.f,
        y,
        buttonWidth,
        buttonHeight);

    button->setFont(font);
    button->setText(text);
    button->setTextSize(30);

    // Optional: Custom colors
    // button->setColors(sf::Color(70, 70, 100), sf::Color(100, 100, 150), sf::Color(50, 50, 80));

    Button *buttonPtr = button.get();
    buttons.push_back(std::move(button));

    return buttonPtr;
}

void Menu::handleMouseMove(sf::Vector2f mousePos)
{
    for (auto &button : buttons)
    {
        button->handleMouseMove(mousePos);
    }
}

void Menu::handleMousePress(sf::Vector2f mousePos)
{
    for (auto &button : buttons)
    {
        button->handleMousePress(mousePos);
    }
}

void Menu::handleMouseRelease()
{
    for (auto &button : buttons)
    {
        button->handleMouseRelease();
    }
}

void Menu::draw(sf::RenderWindow &window)
{
    window.draw(background);

    if (titleText)
    {
        window.draw(*titleText);
    }

    for (auto &button : buttons)
    {
        button->draw(window);
    }
}