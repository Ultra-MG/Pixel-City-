#include "world/Decorations/Fountain.hpp"

sf::Texture Fountain::s_texture;

Fountain::Fountain(int tx, int ty)
{
    x = tx;
    y = ty;
}

void Fountain::loadTexture()
{
    s_texture.loadFromFile("assets/decorations/fountain.png");
}

bool Fountain::canBePlaced(const City &) const
{
    return true;
}

void Fountain::render(sf::RenderTarget &target,const sf::Font& font) const
{
    sf::Sprite s(s_texture);

    s.setPosition(sf::Vector2f{
        float(x * cfg::TileSize),
        float(y * cfg::TileSize)});

    const auto ts = s_texture.getSize();
    s.setScale(sf::Vector2f{
        float(cfg::TileSize) / ts.x,
        float(cfg::TileSize) / ts.y});

    target.draw(s);
}

void Fountain::renderGhost(sf::RenderTarget &target, bool valid) const
{
    sf::Sprite s(s_texture);

    s.setPosition(sf::Vector2f{
        float(x * cfg::TileSize),
        float(y * cfg::TileSize)});

    const auto ts = s_texture.getSize();
    s.setScale(sf::Vector2f{
        float(cfg::TileSize) / ts.x,
        float(cfg::TileSize) / ts.y});

    s.setColor(valid
                   ? sf::Color(200, 255, 200, 180)
                   : sf::Color(255, 0, 0, 180));

    target.draw(s);
}
