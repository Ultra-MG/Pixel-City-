// world/Infrastructure/Bridge.cpp
#include "world/Infrastructure/Bridge.hpp"
#include <SFML/Graphics/Sprite.hpp>

sf::Texture Bridge::s_texture;

Bridge::Bridge(int tx, int ty)
{
    x = tx;
    y = ty;
    w = 1;
    h = 1;
}

void Bridge::loadTexture()
{
    s_texture.loadFromFile("assets/infrastructure/bridge.png");
}

bool Bridge::canBePlaced(const City &) const
{
    return true;
}

void Bridge::render(sf::RenderTarget &target) const
{
    sf::Sprite s(s_texture);

    s.setPosition({float(x * cfg::TileSize),
                   float(y * cfg::TileSize)});

    const auto ts = s_texture.getSize();
    s.setScale({float(cfg::TileSize) / ts.x,
                float(cfg::TileSize) / ts.y});

    target.draw(s);
}

void Bridge::renderGhost(sf::RenderTarget &target, bool valid) const
{
    sf::Sprite s(s_texture);

    s.setPosition({float(x * cfg::TileSize),
                   float(y * cfg::TileSize)});

    const auto ts = s_texture.getSize();
    s.setScale({float(cfg::TileSize) / ts.x,
                float(cfg::TileSize) / ts.y});

    s.setColor(valid
                   ? sf::Color(200, 255, 200, 180)
                   : sf::Color(255, 0, 0, 180));

    target.draw(s);
}
