#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Ground
{
private:
    sf::Texture tileset;
    std::vector<sf::Sprite> tiles;
    std::vector<sf::FloatRect> collisionBoxes;
    int tileWidth;
    int tileHeight;

public:
    Ground(const std::string &tilesetPath, int tileW, int tileH);
    void addTile(float x, float y, int tileIndexX, int tileIndexY);
    void createPlatform(float startX, float y, int length, int tileIndexX, int tileIndexY);
    void draw(sf::RenderWindow &window);
    const std::vector<sf::FloatRect> &getCollisionBoxes() const;
    void clear();
};