// world/Decorations/Tree.cpp
#include "world/Decorations/Tree.hpp"
#include <SFML/Graphics/Sprite.hpp>

sf::Texture Tree::s_texture;

Tree::Tree(int tx, int ty)
{
    x = tx;
    y = ty;
}

void Tree::loadTexture()
{
    s_texture.loadFromFile("assets/decorations/tree.png");
}

bool Tree::canBePlaced(const City &) const
{
    return true;
}

void Tree::render(sf::RenderTarget &target) const
{
    sf::Sprite s(s_texture);

    s.setPosition({float(x * cfg::TileSize),
                   float(y * cfg::TileSize)});

    auto ts = s_texture.getSize();
    s.setScale({float(cfg::TileSize) / ts.x,
                float(cfg::TileSize) / ts.y});

    target.draw(s);
}

void Tree::renderGhost(sf::RenderTarget &target, bool valid) const
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
