#include "world/Buildings/Store.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

sf::Texture Store::s_texture;

Store::Store(int tx, int ty)
{
    x = tx;
    y = ty;
    w = 2;
    h = 2;
}

void Store::loadTexture()
{
    s_texture.loadFromFile("assets/store.png");
}

BuildingType Store::type() const
{
    return BuildingType::Store;
}

bool Store::requiresRoadAccess() const
{
    return true;
}

bool Store::canBePlaced(const City &) const
{
    return true;
}

void Store::render(sf::RenderTarget &target) const
{
    sf::Sprite s(s_texture);

    s.setPosition({float(x * cfg::TileSize),
                   float(y * cfg::TileSize)});

    s.setScale({float(w * cfg::TileSize) / s_texture.getSize().x,
                float(h * cfg::TileSize) / s_texture.getSize().y});

    target.draw(s);
}

void Store::renderGhost(sf::RenderTarget &target, bool valid) const
{
    sf::Sprite s(s_texture);
    std::cout << "Rendering ghost store at (" << x << "," << y << ")\n";

    s.setPosition({float(x * cfg::TileSize),
                   float(y * cfg::TileSize)});

    s.setScale({float(w * cfg::TileSize) / s_texture.getSize().x,
                float(h * cfg::TileSize) / s_texture.getSize().y});

    s.setColor(valid
                   ? sf::Color(200, 255, 200, 180)
                   : sf::Color(255, 0, 0, 180));

    target.draw(s);
}
