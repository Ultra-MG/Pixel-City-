// world/Decorations/LampPost.cpp
#include "world/Decorations/LampPost.hpp"
#include <SFML/Graphics/Sprite.hpp>

sf::Texture LampPost::s_texture;

LampPost::LampPost(int tx, int ty)
{
    x = tx;
    y = ty;
    w = 1;
    h = 1;
}

void LampPost::loadTexture()
{
    s_texture.loadFromFile("assets/decorations/lamppost.png");
}

bool LampPost::canBePlaced(const City &) const
{
    return true;
}

void LampPost::render(sf::RenderTarget &target, const sf::Font& font) const
{
    sf::Sprite s(s_texture);

    s.setPosition({float(x * cfg::TileSize),
                   float(y * cfg::TileSize)});

    auto ts = s_texture.getSize();
    s.setScale({float(cfg::TileSize) / ts.x,
                float(cfg::TileSize) / ts.y});

    target.draw(s);
}

void LampPost::renderGhost(sf::RenderTarget &target, bool valid) const
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
