#include "world/Buildings/House.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

sf::Texture House::s_texture;

House::House(int tx, int ty)
{
    x = tx;
    y = ty;
    w = 2;
    h = 2;
}

void House::loadTexture()
{
    s_texture.loadFromFile("assets/house.png");
}

BuildingType House::type() const
{
    return BuildingType::House;
}

bool House::requiresRoadAccess() const
{
    return true;
}

bool House::canBePlaced(const City &) const
{
    return true;
}

void House::render(sf::RenderTarget &target, const sf::Font& font) const
{
    sf::Sprite s(s_texture);

    s.setPosition(sf::Vector2f{
        float(x * cfg::TileSize),
        float(y * cfg::TileSize)});

    s.setScale(sf::Vector2f{
        float(w * cfg::TileSize) / s_texture.getSize().x,
        float(h * cfg::TileSize) / s_texture.getSize().y});

    target.draw(s);
}

void House::renderGhost(sf::RenderTarget &target, bool valid) const
{
    sf::Sprite s(s_texture);
    std::cout << "Rendering ghost house at (" << x << "," << y << ")\n";

    s.setPosition(sf::Vector2f{
        float(x * cfg::TileSize),
        float(y * cfg::TileSize)});

    s.setScale(sf::Vector2f{
        float(w * cfg::TileSize) / s_texture.getSize().x,
        float(h * cfg::TileSize) / s_texture.getSize().y});

    s.setColor(valid
                   ? sf::Color(200, 255, 200, 180)
                   : sf::Color(255, 0, 0, 180));

    target.draw(s);
}
