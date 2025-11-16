#include <iostream>
#include "scenes/MenuScene.hpp"
#include "Constants.hpp"

MenuScene::MenuScene(float windowWidth, float windowHeight)
    : m_nextState(GameState::Menu)
{
    m_background.setSize({windowWidth, windowHeight});
    m_background.setFillColor(sf::Color(30, 30, 50));

    // Font
    if (!loadFont(Paths::FONT_PATH))
    {
        std::cerr << "Failed to load font for menu!" << std::endl;
    }

    // Buttons
    Button *playButton = addButton("PLAY", 300.f);
    playButton->setOnClick([this]()
                           {
        m_nextState = GameState::Playing; // Set state untuk diganti
        std::cout << "Starting game..." << std::endl; });

    Button *optionsButton = addButton("OPTIONS", 380.f);
    optionsButton->setOnClick([]()
                              { std::cout << "Options clicked!" << std::endl; });

    Button *exitButton = addButton("EXIT", 460.f);
    exitButton->setOnClick([this]()
                           {
                               m_nextState = GameState::Exit; // Set state untuk keluar
                           });
}

void MenuScene::handleEvent(const sf::Event &event, sf::RenderWindow &window)
{
    // Dapatkan posisi mouse dari sini
    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(mousePixelPos);

    if (event.is<sf::Event::Closed>())
    {
        m_nextState = GameState::Exit;
    }

    // Panggil helper Anda yang sudah ada berdasarkan event
    if (event.is<sf::Event::MouseMoved>())
    {
        handleMouseMove(mousePos);
    }

    if (const auto *mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePressed->button == sf::Mouse::Button::Left)
        {
            handleMousePress(mousePos);
        }
    }

    if (const auto *mouseReleased = event.getIf<sf::Event::MouseButtonReleased>())
    {
        if (mouseReleased->button == sf::Mouse::Button::Left)
        {
            handleMouseRelease();
        }
    }
}

GameState MenuScene::update(float deltaTime)
{
    return m_nextState;
}

bool MenuScene::loadFont(const std::string &fontPath)
{
    if (!m_font.openFromFile(fontPath))
    {
        std::cerr << "Error loading font!" << std::endl;
        return false;
    }

    m_titleText.emplace(m_font, "I am not a hero", 80);
    m_titleText->setFillColor(sf::Color::White);
    m_titleText->setStyle(sf::Text::Bold);

    sf::FloatRect titleBounds = m_titleText->getLocalBounds();
    m_titleText->setOrigin({titleBounds.size.x / 2.f, titleBounds.size.y / 2.f});
    m_titleText->setPosition({m_background.getSize().x / 2.f, 150.f});

    return true;
}

Button *MenuScene::addButton(const std::string &text, float y)
{
    float centerX = m_background.getSize().x / 2.f;
    float buttonWidth = 200.f;
    float buttonHeight = 60.f;

    auto button = std::make_unique<Button>(
        centerX - buttonWidth / 2.f,
        y,
        buttonWidth,
        buttonHeight);

    button->setFont(m_font);
    button->setText(text);
    button->setTextSize(30);

    Button *buttonPtr = button.get();
    m_buttons.push_back(std::move(button));

    return buttonPtr;
}

void MenuScene::handleMouseMove(sf::Vector2f mousePos)
{
    for (auto &button : m_buttons)
    {
        button->handleMouseMove(mousePos);
    }
}

void MenuScene::handleMousePress(sf::Vector2f mousePos)
{
    for (auto &button : m_buttons)
    {
        button->handleMousePress(mousePos);
    }
}

void MenuScene::handleMouseRelease()
{
    for (auto &button : m_buttons)
    {
        button->handleMouseRelease();
    }
}

// Fungsi draw Anda (tambahkan reset view)
void MenuScene::draw(sf::RenderWindow &window)
{
    // Penting: Reset view ke default untuk menggambar UI
    window.setView(window.getDefaultView());

    window.draw(m_background);

    if (m_titleText)
    {
        window.draw(*m_titleText);
    }

    for (auto &button : m_buttons)
    {
        button->draw(window);
    }
}