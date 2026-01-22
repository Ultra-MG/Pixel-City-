#include "world/Crops/Corn.hpp"
#include <SFML/Graphics/Sprite.hpp>

sf::Texture Corn::s_texture;

void Corn::loadTexture() {
    s_texture.loadFromFile("assets/crops/corn.png");
}

void Corn::render(sf::RenderTarget& target, int x, int y) const {
    sf::Sprite s(s_texture);
    s.setPosition(sf::Vector2f{
        float(x * cfg::TileSize),
        float(y * cfg::TileSize)});
    const auto ts = s_texture.getSize();
    if (ts.x > 0 && ts.y > 0) {
        s.setScale(sf::Vector2f{
            float(cfg::TileSize) / ts.x,
            float(cfg::TileSize) / ts.y});
    }
    target.draw(s);
}