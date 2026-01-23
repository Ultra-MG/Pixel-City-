#include "world/Buildings/TownHall.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include "core/Config.hpp"

sf::Texture TownHall::s_texture;

TownHall::TownHall(int tx, int ty)
{
    x = tx;
    y = ty;
    w = 4;
    h = 4;
    level = 1;
    m_storedMoney = 0;
}

void TownHall::loadTexture()
{
    s_texture.loadFromFile("assets/townhall.png");
}

BuildingType TownHall::type() const
{
    return BuildingType::TownHall;
}

bool TownHall::requiresRoadAccess() const
{
    return true;
}

void TownHall::tick(std::int64_t seconds)
{
    MoneyProducer::tick(seconds);
}

int TownHall::collectMoney()
{
    return collect();
}

bool TownHall::canBePlaced(const City &) const
{
    return true;
}

int TownHall::moneyPerMinute() const
{
    return 10 * level;
}

int TownHall::storedMoney() const
{
    return m_storedMoney;
}

void TownHall::setStoredMoney(int v)
{
    m_storedMoney = v;
}

void TownHall::applyOffline(std::int64_t seconds)
{
    MoneyProducer::applyOffline(seconds);
}

void TownHall::saveTo(PlacedObject &out) const
{
    Building::saveTo(out);
    out.data["level"] = std::to_string(level);
    out.data["money"] = std::to_string(m_storedMoney);
}

void TownHall::loadFrom(const PlacedObject &in)
{
    Building::loadFrom(in);

    if (auto it = in.data.find("level"); it != in.data.end())
        level = std::stoi(it->second);
    else
        level = 1;

    if (auto it = in.data.find("money"); it != in.data.end())
        m_storedMoney = std::stoi(it->second);
    else
        m_storedMoney = 0;
}

// ===== Rendering =====

void TownHall::render(sf::RenderTarget &target,const sf::Font& font) const
{
    sf::Sprite s(s_texture);
    s.setPosition({float(x * cfg::TileSize),
                   float(y * cfg::TileSize)});
    s.setScale({float(w * cfg::TileSize) / s_texture.getSize().x,
                float(h * cfg::TileSize) / s_texture.getSize().y});
    target.draw(s);

    if (m_storedMoney <= 0)
        return;

    sf::RectangleShape badge;
    badge.setSize({float(w * cfg::TileSize), 10.f});
    badge.setFillColor(sf::Color(0, 0, 0, 160));
    badge.setPosition({float(x * cfg::TileSize),
                       float(y * cfg::TileSize) - 12.f});
    target.draw(badge);

    sf::Text txt(font);
    txt.setCharacterSize(9);
    txt.setFillColor(sf::Color(255, 220, 120));
    txt.setString("$" + std::to_string(m_storedMoney));
    txt.setPosition({float(x * cfg::TileSize) + 2.f,
                     float(y * cfg::TileSize) - 11.f});
    target.draw(txt);
}

void TownHall::renderGhost(sf::RenderTarget &target, bool valid) const
{
    sf::Sprite s(s_texture);
    s.setPosition({float(x * cfg::TileSize),
                   float(y * cfg::TileSize)});
    s.setScale({float(w * cfg::TileSize) / s_texture.getSize().x,
                float(h * cfg::TileSize) / s_texture.getSize().y});
    s.setColor(valid
                   ? sf::Color(200, 255, 200, 180)
                   : sf::Color(255, 0, 0, 180));
    target.draw(s);
}
