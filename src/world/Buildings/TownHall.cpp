#include "world/Buildings/TownHall.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include "game/Inventory.hpp"
#include "core/Config.hpp"

sf::Texture TownHall::s_texture;

TownHall::TownHall(int tx, int ty)
{
    x = tx;
    y = ty;
    w = 4;
    h = 4;
    m_level = 1;
    m_storedMoney = 0;
}

Cost TownHall::upgradeCost() const
{
    return Cost(Currency::Money, 100 * m_level);
}

void TownHall::upgrade(const City &city)
{
    if (!canUpgrade(city))
        return;

    ++m_level;
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

void TownHall::tick(std::int64_t seconds,Inventory& inventory)
{
    MoneyProducer::tick(seconds , inventory);
}

bool TownHall::canBePlaced(const City &) const
{
    return true;
}

int TownHall::moneyPerMinute() const
{
    return 10 * m_level;
}

int TownHall::storedMoney() const
{
    return m_storedMoney;
}

void TownHall::setStoredMoney(int v)
{
    m_storedMoney = v;
}

void TownHall::applyOffline(std::int64_t seconds, Inventory& inventory)
{
    MoneyProducer::applyOffline(seconds, inventory);
}

void TownHall::saveTo(PlacedObject &out) const
{
    Building::saveTo(out);
    out.data["money"] = std::to_string(m_storedMoney);
}

void TownHall::loadFrom(const PlacedObject &in)
{
    Building::loadFrom(in);

    if (auto it = in.data.find("money"); it != in.data.end())
        m_storedMoney = std::stoi(it->second);
    else
        m_storedMoney = 0;
}

void TownHall::render(sf::RenderTarget &target, const sf::Font &font) const
{

    drawLevelBadge(*this, target, font);
    sf::Sprite s(s_texture);
    s.setPosition({float(x * cfg::TileSize),
                   float(y * cfg::TileSize)});
    s.setScale({float(w * cfg::TileSize) / s_texture.getSize().x,
                float(h * cfg::TileSize) / s_texture.getSize().y});
    target.draw(s);

}
