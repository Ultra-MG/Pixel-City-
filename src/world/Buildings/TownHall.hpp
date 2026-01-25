#pragma once

#include "world/Building.hpp"
#include "world/MoneyProducer.hpp"
#include "core/Config.hpp"
#include "world/City.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Font.hpp>
#include "game/Cost.hpp"

class Inventory;
class TownHall : public Building, public MoneyProducer
{
public:
    TownHall(int tx, int ty);

    static void loadTexture();

    Cost cost() const override { return {Currency::Money, 100}; }
    BuildingType type() const override;
    bool requiresRoadAccess() const override;
    bool canBePlaced(const City &) const override;

    BuildTool buildTool() const override
    {
        return BuildTool::PlaceTownHall;
    }

    int moneyPerMinute() const override;
    int storedMoney() const override;
    void setStoredMoney(int v) override;
    void applyOffline(std::int64_t seconds, Inventory& inventory) override;
    void tick(std::int64_t seconds, Inventory& inventory) override;
    bool canBeDeleted() const override { return false; }

    void saveTo(PlacedObject &out) const override;
    void loadFrom(const PlacedObject &in) override;

    void render(sf::RenderTarget &target, const sf::Font &font) const override;

    bool canUpgrade(const City &city) const override
    {
        return m_level < maxLevel();
    }

    int maxLevel() const override { return 5; }

    Cost upgradeCost() const override;
    void upgrade(const City &city) override;
    bool upgradable() const override { return true; }
    void renderGhost(sf::RenderTarget &, bool) const override {}
    int baseStorage() const override { return 500; }
    int storagePerLevel() const override { return 500; }
    int maxStorage() const override {
        return baseStorage() + storagePerLevel() * m_level;
    }
    

private:
    static sf::Texture s_texture;
    int m_storedMoney = 0;
};
