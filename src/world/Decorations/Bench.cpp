// world/Decorations/Bench.cpp
#include "world/Decorations/Bench.hpp"
#include <SFML/Graphics/Sprite.hpp>

sf::Texture Bench::s_texture;

Bench::Bench(int tx, int ty)
{
    x = tx;
    y = ty;
}

void Bench::loadTexture()
{
    s_texture.loadFromFile("assets/decorations/bench.png");
}

bool Bench::canBePlaced(const City &) const
{
    return true;
}

void Bench::render(sf::RenderTarget &target) const
{
    sf::Sprite s(s_texture);

    s.setPosition({float(x * cfg::TileSize),
                   float(y * cfg::TileSize)});

    auto ts = s_texture.getSize();
    s.setScale({float(cfg::TileSize) / ts.x,
                float(cfg::TileSize) / ts.y});

    target.draw(s);
}

void Bench::renderGhost(sf::RenderTarget &target, bool valid) const
{
    sf::Sprite s(s_texture);

    s.setPosition({float(x * cfg::TileSize),
                   float(y * cfg::TileSize)});

    auto ts = s_texture.getSize();
    s.setScale({float(cfg::TileSize) / ts.x,
                float(cfg::TileSize) / ts.y});

    s.setColor(valid
                   ? sf::Color(200, 255, 200, 180)
                   : sf::Color(255, 0, 0, 180));

    target.draw(s);
}
