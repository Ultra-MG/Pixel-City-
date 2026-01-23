#include "world/Buildings/Farm.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include "core/Config.hpp"

sf::Texture Farm::s_texture;

Farm::Farm(int tx, int ty)
{
    x = tx;
    y = ty;
    w = 3;
    h = 3;
    m_storedMoney = 0;
}

void Farm::loadTexture()
{
    s_texture.loadFromFile("assets/farm.png");
}


BuildingType Farm::type() const
{
    return BuildingType::Farm;
}

bool Farm::requiresRoadAccess() const
{
    return false;
}

bool Farm::canBePlaced(const City &) const
{
    return true;
}

int Farm::moneyPerMinute() const
{
    return 5;
}

int Farm::storedMoney() const
{
    return m_storedMoney;
}

void Farm::setStoredMoney(int v)
{
    m_storedMoney = v;
}

void Farm::applyOffline(std::int64_t seconds)
{
    MoneyProducer::applyOffline(seconds);
}

void Farm::tick(std::int64_t seconds)
{
    MoneyProducer::tick(seconds);
}

int Farm::collectMoney()
{
    return collect();
}
void Farm::saveTo(PlacedObject &out) const
{
    Building::saveTo(out);
    out.data["money"] = std::to_string(m_storedMoney);
}

void Farm::loadFrom(const PlacedObject &in)
{
    Building::loadFrom(in);

    if (auto it = in.data.find("money"); it != in.data.end())
        m_storedMoney = std::stoi(it->second);
    else
        m_storedMoney = 0;
}

void Farm::render(sf::RenderTarget &target,const sf::Font& font ) const
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

void Farm::renderGhost(sf::RenderTarget &target, bool valid) const
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
