#include "world/WorldObject.hpp"
#include "core/Config.hpp"
#include "world/City.hpp"

void WorldObject::saveTo(PlacedObject &out) const
{
    out.x = x;
    out.y = y;
    out.data["level"] = std::to_string(m_level);
}

void WorldObject::loadFrom(const PlacedObject &in)
{
    x = in.x;
    y = in.y;

    if (auto it = in.data.find("level"); it != in.data.end())
        m_level = std::stoi(it->second);
    else
        m_level = 1;
}

void drawCoinBadge(const WorldObject &obj, sf::RenderTarget &target,
                   const sf::Font &font, const int m_storedMoney)
{
    if (m_storedMoney <= 0)
        return;

    sf::RectangleShape badge;
    badge.setSize({float(obj.w * cfg::TileSize), 10.f});
    badge.setFillColor(sf::Color(0, 0, 0, 160));
    badge.setPosition({float(obj.x * cfg::TileSize),
                       float(obj.y * cfg::TileSize) - 16.f});
    target.draw(badge);

    sf::Text txt(font);
    txt.setCharacterSize(9);
    txt.setFillColor(sf::Color(255, 220, 120));
    txt.setString("$" + std::to_string(m_storedMoney));
    txt.setPosition({float(obj.x * cfg::TileSize) + 2.f,
                     float(obj.y * cfg::TileSize) - 16.f});
    target.draw(txt);
}

void drawLevelBadge(const WorldObject &obj,
                    sf::RenderTarget &target,
                    const sf::Font &font)
{
    if (obj.maxLevel() <= 1)
        return;

    sf::RectangleShape bg;
    bg.setSize({14.f, 10.f});
    bg.setFillColor(sf::Color(0, 0, 0, 160));
    bg.setPosition(sf::Vector2f(
        float(obj.x * cfg::TileSize),
        float(obj.y * cfg::TileSize) - 10.f));

    target.draw(bg);

    sf::Text txt(font);
    txt.setCharacterSize(8);
    txt.setFillColor(
        obj.upgradable()
            ? sf::Color::White
            : sf::Color(255, 200, 120));

    txt.setString(
        obj.upgradable()
            ? "L" + std::to_string(obj.level())
            : "MAX");

    txt.setPosition({float(obj.x * cfg::TileSize) + 2.f,
                     float(obj.y * cfg::TileSize) - 10.f});

    target.draw(txt);
}
