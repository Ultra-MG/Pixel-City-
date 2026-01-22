// world/Buildings/Store.cpp
#include "world/Buildings/Store.hpp"
#include "game/EconomySystem.hpp"
#include <SFML/Graphics/Sprite.hpp>

sf::Texture Store::s_texture;

Store::Store(int tx, int ty)
{
    x = tx;
    y = ty;
    w = 2;
    h = 2;
    level = 1;
    m_storedMoney = 0;
}

void Store::loadTexture()
{
    s_texture.loadFromFile("assets/store.png");
}

BuildingType Store::type() const
{
    return BuildingType::Store;
}

bool Store::requiresRoadAccess() const
{
    return true;
}

bool Store::canBePlaced(const City &) const
{
    return true;
}

// ===== MoneyProducer =====

int Store::moneyPerMinute() const
{
    return 5 * level;
}

int Store::storedMoney() const
{
    return m_storedMoney;
}

void Store::setStoredMoney(int v)
{
    m_storedMoney = v;
}

void Store::applyOffline(std::int64_t seconds)
{
    MoneyProducer::applyOffline(seconds);
}

void Store::tick(std::int64_t seconds, EconomySystem& eco)
{
    const int produced =
        static_cast<int>((seconds * moneyPerMinute()) / 60);

    if (produced > 0)
        eco.addMoney(produced);
}

// ===== Save / Load =====

void Store::saveTo(PlacedObject& out) const
{
    Building::saveTo(out);
    out.data["level"] = std::to_string(level);
    out.data["money"] = std::to_string(m_storedMoney);
}

void Store::loadFrom(const PlacedObject& in)
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

void Store::render(sf::RenderTarget &target) const
{
    sf::Sprite s(s_texture);

    s.setPosition({float(x * cfg::TileSize),
                   float(y * cfg::TileSize)});

    s.setScale({float(w * cfg::TileSize) / s_texture.getSize().x,
                float(h * cfg::TileSize) / s_texture.getSize().y});

    target.draw(s);
}

void Store::renderGhost(sf::RenderTarget &target, bool valid) const
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
