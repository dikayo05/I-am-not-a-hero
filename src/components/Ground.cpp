#include "components/Ground.hpp"

Ground::Ground(const std::string &tilesetPath, int tileW = 32, int tileH = 32)
    : m_tileWidth(tileW), m_tileHeight(tileH)
{
    if (!m_tileset.loadFromFile(tilesetPath))
    {
        std::cerr << "Error loading tileset!" << std::endl;
    }
}

// add single tile at (x, y) with tile index in tileset (tileIndexX, tileIndexY)
void Ground::addTile(float x, float y, int tileIndexX = 0, int tileIndexY = 0)
{
    sf::Sprite tile(m_tileset);

    // Set texture rect for the tile
    tile.setTextureRect(sf::IntRect(
        {tileIndexX * m_tileWidth, tileIndexY * m_tileHeight},
        {m_tileWidth, m_tileHeight}));

    tile.setPosition({x, y});
    m_tiles.push_back(tile);

    // add collision box
    m_collisionBoxes.push_back(sf::FloatRect({x, y}, {static_cast<float>(m_tileWidth), static_cast<float>(m_tileHeight)}));
}

// make horizontal platform
void Ground::createPlatform(float startX, float y, int length, int tileIndexX = 0, int tileIndexY = 0)
{
    for (int i = 0; i < length; i++)
    {
        addTile(startX + (i * m_tileWidth), y, tileIndexX, tileIndexY);
    }
}

void Ground::draw(sf::RenderWindow &window)
{
    for (const auto &tile : m_tiles)
    {
        window.draw(tile);
    }
}

const std::vector<sf::FloatRect> &Ground::getCollisionBoxes() const
{
    return m_collisionBoxes;
}

void Ground::clear()
{
    m_tiles.clear();
    m_collisionBoxes.clear();
}