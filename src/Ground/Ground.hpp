#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Ground
{
private:
    sf::Texture m_tileset;
    std::vector<sf::Sprite> m_tiles;
    std::vector<sf::FloatRect> m_collisionBoxes;
    int m_tileWidth;
    int m_tileHeight;

public:
    Ground(const std::string &tilesetPath, int tileW, int tileH);
    void addTile(float x, float y, int tileIndexX, int tileIndexY);
    void createPlatform(float startX, float y, int length, int tileIndexX, int tileIndexY);
    void draw(sf::RenderWindow &window);
    const std::vector<sf::FloatRect> &getCollisionBoxes() const;
    void clear();
};