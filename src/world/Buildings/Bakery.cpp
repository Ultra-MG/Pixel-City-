#include "world/Buildings/Bakery.hpp"
#include "game/Inventory.hpp"
#include "core/Config.hpp"
#include "world/City.hpp"
#include <SFML/Graphics/Sprite.hpp>

sf::Texture Bakery::s_texture;

Bakery::Bakery(int tx, int ty)
{
    x = tx;
    y = ty;
    w = 3;
    h = 3;
    m_storedMoney = 0;
}

void Bakery::loadTexture()
{
    s_texture.loadFromFile("assets/bakery.png");
}

int Bakery::moneyPerMinute() const
{
    return 8 * m_level;
}

int Bakery::storedMoney() const
{
    return m_storedMoney;
}

void Bakery::setStoredMoney(int v)
{
    m_storedMoney = v;
}

void Bakery::tick(std::int64_t seconds, Inventory& inventory)
{
    const std::int64_t ticks = seconds;          
    const int wheatNeeded = ticks / 10;          

    if (wheatNeeded <= 0)
        return;

    if (!inventory.remove("wheat", wheatNeeded))
        return;

    MoneyProducer::tick(wheatNeeded * 10,inventory);
}


void Bakery::saveTo(PlacedObject &out) const
{
    Building::saveTo(out);
    out.data["money"] = std::to_string(m_storedMoney);
}
void Bakery::renderGhost(sf::RenderTarget &target, bool valid) const
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

Cost Bakery::upgradeCost() const
{
    return {Currency::Money, 100 * m_level};
}


void Bakery::upgrade(const City &city)
{
    if (!canUpgrade(city))
        return;

    ++m_level;
}

bool Bakery::requiresRoadAccess() const
{
    return true;
}

bool Bakery::canBePlaced(const City &) const
{
    return true;
}

void Bakery::applyOffline(std::int64_t seconds, Inventory &inventory)
{
    MoneyProducer::applyOffline(seconds, inventory);
}

int Bakery::collectMoney()
{
    return collect().money;
}

void Bakery::loadFrom(const PlacedObject &in)
{
    Building::loadFrom(in);

    if (auto it = in.data.find("money"); it != in.data.end())
        m_storedMoney = std::stoi(it->second);
    else
        m_storedMoney = 0;
}

void Bakery::render(sf::RenderTarget &target, const sf::Font &font) const
{

    drawLevelBadge(*this, target, font);
    sf::Sprite s(s_texture);

    s.setPosition({float(x * cfg::TileSize),
                   float(y * cfg::TileSize)});

    s.setScale({float(w * cfg::TileSize) / s_texture.getSize().x,
                float(h * cfg::TileSize) / s_texture.getSize().y});

    target.draw(s);
}
int Bakery::effectiveMaxLevel(const City &city) const
{
    return std::min(maxLevel(), city.townHallLevel());
}
