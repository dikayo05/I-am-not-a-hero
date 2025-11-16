#include <iostream>
#include "Game.hpp"
#include "Constants.hpp"
#include "scenes/MenuScene.hpp"
#include "scenes/GameScene.hpp"

Game::Game()
    : m_window(sf::VideoMode({Paths::WINDOW_WIDTH, Paths::WINDOW_HEIGHT}), "I am not a hero"),
      m_currentState(GameState::Menu)
{
    m_window.setFramerateLimit(60);
    // Langsung ganti ke scene menu saat game dimulai
    changeScene(m_currentState);
}

void Game::changeScene(GameState newState)
{
    // Hapus scene lama dan buat scene baru
    if (newState == GameState::Menu)
    {
        m_currentScene = std::make_unique<MenuScene>(Paths::WINDOW_WIDTH, Paths::WINDOW_HEIGHT);
    }
    else if (newState == GameState::Playing)
    {
        m_currentScene = std::make_unique<GameScene>();
    }
    else if (newState == GameState::Exit)
    {
        m_window.close();
    }
    m_currentState = newState;
}

void Game::run()
{
    // Ini adalah Game Loop Anda yang baru, jauh lebih bersih
    while (m_window.isOpen())
    {
        float deltaTime = m_clock.restart().asSeconds();

        handleEvents();
        update(deltaTime);
        render();
    }
}

void Game::handleEvents()
{
    while (const std::optional event = m_window.pollEvent())
    {
        // Kirim event ke scene aktif untuk diproses
        if (m_currentScene)
        {
            m_currentScene->handleEvent(*event, m_window);
        }
    }
}

void Game::update(float deltaTime)
{
    if (!m_currentScene)
        return;

    // Minta scene aktif untuk update
    GameState nextState = m_currentScene->update(deltaTime);

    // Jika scene meminta ganti state, ganti scenenya
    if (nextState != m_currentState)
    {
        changeScene(nextState);
    }
}

void Game::render()
{
    // Warna clear default, bisa dipindah ke scene
    m_window.clear(sf::Color(135, 206, 235));

    if (m_currentScene)
    {
        // Minta scene aktif untuk menggambar
        m_currentScene->draw(m_window);
    }

    m_window.display();
}