#include "Ground.hpp"

Ground::Ground(const std::string &tilesetPath, int tileW = 32, int tileH = 32)
    : tileWidth(tileW), tileHeight(tileH)
{
    if (!tileset.loadFromFile(tilesetPath))
    {
        std::cerr << "Error loading tileset!" << std::endl;
    }
}

// Tambah tile di posisi tertentu
void Ground::addTile(float x, float y, int tileIndexX = 0, int tileIndexY = 0)
{
    sf::Sprite tile(tileset);

    // Set texture rect untuk memilih tile dari tileset
    tile.setTextureRect(sf::IntRect(
        {tileIndexX * tileWidth, tileIndexY * tileHeight},
        {tileWidth, tileHeight}));

    tile.setPosition({x, y});
    tiles.push_back(tile);

    // Tambah collision box
    collisionBoxes.push_back(sf::FloatRect({x, y}, {static_cast<float>(tileWidth), static_cast<float>(tileHeight)}));
}

// Buat platform horizontal
void Ground::createPlatform(float startX, float y, int length, int tileIndexX = 0, int tileIndexY = 0)
{
    for (int i = 0; i < length; i++)
    {
        addTile(startX + (i * tileWidth), y, tileIndexX, tileIndexY);
    }
}

void Ground::draw(sf::RenderWindow &window)
{
    for (const auto &tile : tiles)
    {
        window.draw(tile);
    }
}

const std::vector<sf::FloatRect> &Ground::getCollisionBoxes() const
{
    return collisionBoxes;
}

void Ground::clear()
{
    tiles.clear();
    collisionBoxes.clear();
}