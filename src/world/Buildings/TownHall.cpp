#include "world/Buildings/TownHall.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

sf::Texture TownHall::s_texture;

TownHall::TownHall(int tx, int ty)
{
    x = tx;
    y = ty;
    w = 4;
    h = 4;
}

void TownHall::loadTexture()
{
    s_texture.loadFromFile("assets/townhall.png");
}

BuildingType TownHall::type() const
{
    return BuildingType::TownHall;
}

bool TownHall::requiresRoadAccess() const
{
    return true;
}

bool TownHall::canBePlaced(const City &) const
{
    return true;
}

void TownHall::render(sf::RenderTarget &target) const
{
    sf::Sprite s(s_texture);

    s.setPosition({float(x * cfg::TileSize),
                   float(y * cfg::TileSize)});

    s.setScale({float(w * cfg::TileSize) / s_texture.getSize().x,
                float(h * cfg::TileSize) / s_texture.getSize().y});

    target.draw(s);
}

void TownHall::renderGhost(sf::RenderTarget &target, bool valid) const
{
    sf::Sprite s(s_texture);
    std::cout << "Rendering ghost town hall at (" << x << "," << y << ")\n";

    s.setPosition({float(x * cfg::TileSize),
                   float(y * cfg::TileSize)});

    s.setScale({float(w * cfg::TileSize) / s_texture.getSize().x,
                float(h * cfg::TileSize) / s_texture.getSize().y});

    s.setColor(valid
                   ? sf::Color(200, 255, 200, 180)
                   : sf::Color(255, 0, 0, 180));

    target.draw(s);
}
