#pragma once

#include "world/Building.hpp"
#include "world/MoneyProducer.hpp"
#include "core/Config.hpp"
#include "world/City.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Font.hpp>
#include "game/Cost.hpp"

class TownHall : public Building, public MoneyProducer {
public:
    TownHall(int tx, int ty);

    static void loadTexture();
    static void loadFont();

    Cost cost() const override { return {Currency::Money, 100}; }
    BuildingType type() const override;
    bool requiresRoadAccess() const override;
    bool canBePlaced(const City&) const override;

    BuildTool buildTool() const override
    {
        return BuildTool::PlaceTownHall;
    }

    // MoneyProducer
    int moneyPerMinute() const override;
    int storedMoney() const override;
    void setStoredMoney(int v) override;
    void applyOffline(std::int64_t seconds) override;
    void tick(std::int64_t seconds, EconomySystem& eco) override;

    // Save
    void saveTo(PlacedObject& out) const override;
    void loadFrom(const PlacedObject& in) override;

    void render(sf::RenderTarget& target) const override;
    void renderGhost(sf::RenderTarget& target, bool valid) const override;

private:
    static sf::Texture s_texture;
    static sf::Font s_font;

    int m_storedMoney = 0;
};
