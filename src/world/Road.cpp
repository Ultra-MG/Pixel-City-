// world/Road.cpp
#include "world/Road.hpp"
#include <SFML/Graphics/Sprite.hpp>

sf::Texture Road::s_texture;

Road::Road(int tx, int ty) {
    x = tx;
    y = ty;
    w = 1;
    h = 1;
}

void Road::loadTexture() {
    s_texture.loadFromFile("assets/road.png");
}

bool Road::canBePlaced(const City&) const {
    return true;
}

void Road::render(sf::RenderTarget& target) const {
    sf::Sprite s(s_texture);

    s.setPosition(sf::Vector2f{
        float(x * cfg::TileSize),
        float(y * cfg::TileSize)
    });

    const auto ts = s_texture.getSize();
    s.setScale(sf::Vector2f{
        float(cfg::TileSize) / ts.x,
        float(cfg::TileSize) / ts.y
    });

    target.draw(s);
}

void Road::renderGhost(sf::RenderTarget& target, bool valid) const {
    sf::Sprite s(s_texture);

    s.setPosition(sf::Vector2f{
        float(x * cfg::TileSize),
        float(y * cfg::TileSize)
    });

    const auto ts = s_texture.getSize();
    s.setScale(sf::Vector2f{
        float(cfg::TileSize) / ts.x,
        float(cfg::TileSize) / ts.y
    });

    s.setColor(valid
        ? sf::Color(200, 255, 200, 180)
        : sf::Color(255, 200, 200, 180));

    target.draw(s);
}
