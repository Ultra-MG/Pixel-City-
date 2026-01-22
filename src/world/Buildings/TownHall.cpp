#include "world/Buildings/TownHall.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

sf::Texture TownHall::s_texture;
sf::Font TownHall::s_font;

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

void TownHall::loadFont()
{
    s_font.openFromFile("assets/fonts/pixelFont.ttf");
}

BuildingType TownHall::type() const
{
    return BuildingType::TownHall;
}

bool TownHall::requiresRoadAccess() const
{
    return true;
}

void TownHall::tick(std::int64_t seconds, EconomySystem &eco)
{
    const int produced =
        static_cast<int>((seconds * moneyPerMinute()) / 60);

    if (produced > 0)
        eco.addMoney(produced);
}

bool TownHall::canBePlaced(const City &) const
{
    return true;
}

// ===== MoneyProducer =====

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

// ===== Save / Load =====

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

void TownHall::render(sf::RenderTarget &target) const
{
    sf::Sprite s(s_texture);
    s.setPosition({float(x * cfg::TileSize),
                   float(y * cfg::TileSize)});
    s.setScale({float(w * cfg::TileSize) / s_texture.getSize().x,
                float(h * cfg::TileSize) / s_texture.getSize().y});
    target.draw(s);

    sf::Text txt(s_font);
    txt.setCharacterSize(10);
    txt.setFillColor(sf::Color::White);
    txt.setString("Lv " + std::to_string(level) +
                  " $" + std::to_string(m_storedMoney));

    txt.setPosition({float(x * cfg::TileSize),
                     float(y * cfg::TileSize) - 12.f});

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
