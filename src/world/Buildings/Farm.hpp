// world/Farm.hpp
#pragma once

#include "world/Building.hpp"
#include "world/MoneyProducer.hpp"
#include "core/Config.hpp"
#include "world/City.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "game/Cost.hpp"

class Farm : public Building, public MoneyProducer
{
public:
    Farm(int tx, int ty);

    static void loadTexture();

    Cost cost() const override { return {Currency::Money, 20}; }
    BuildingType type() const override;
    bool requiresRoadAccess() const override;
    bool canBePlaced(const City &) const override;

    BuildTool buildTool() const override
    {
        return BuildTool::PlaceFarm;
    }

    int moneyPerMinute() const override;
    int storedMoney() const override;
    void setStoredMoney(int v) override;
    void applyOffline(std::int64_t seconds) override;
    void tick(std::int64_t seconds) override;
    int collectMoney();

    // Save / Load
    void saveTo(PlacedObject &out) const override;
    void loadFrom(const PlacedObject &in) override;

    void render(sf::RenderTarget &target, const sf::Font& font) const override;
    void renderGhost(sf::RenderTarget &target, bool valid) const override;

private:
    static sf::Texture s_texture;
    int m_storedMoney = 0;
};
