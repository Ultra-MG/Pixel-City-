#include "world/Buildings/Farm.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

sf::Texture Farm::s_texture;

Farm::Farm(int tx, int ty)
{
    x = tx;
    y = ty;
    w = 3;
    h = 3;
}

void Farm::loadTexture()
{
    s_texture.loadFromFile("assets/farm.png");
}

BuildingType Farm::type() const
{
    return BuildingType::Farm;
}

bool Farm::requiresRoadAccess() const
{
    return false;
}

bool Farm::canBePlaced(const City &) const
{
    return true;
}

void Farm::render(sf::RenderTarget &target) const
{
    sf::Sprite s(s_texture);

    s.setPosition({float(x * cfg::TileSize),
                   float(y * cfg::TileSize)});

    s.setScale({float(w * cfg::TileSize) / s_texture.getSize().x,
                float(h * cfg::TileSize) / s_texture.getSize().y});

    target.draw(s);
}

void Farm::renderGhost(sf::RenderTarget &target, bool valid) const
{
    sf::Sprite s(s_texture);
    std::cout << "Rendering ghost farm at (" << x << "," << y << ")\n";

    s.setPosition({float(x * cfg::TileSize),
                   float(y * cfg::TileSize)});

    s.setScale({float(w * cfg::TileSize) / s_texture.getSize().x,
                float(h * cfg::TileSize) / s_texture.getSize().y});

    s.setColor(valid
                   ? sf::Color(200, 255, 200, 180)
                   : sf::Color(255, 0, 0, 180));

    target.draw(s);
}
