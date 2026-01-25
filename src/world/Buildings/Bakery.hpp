#pragma once
#include "world/Building.hpp"
#include "world/MoneyProducer.hpp"

class Inventory;
class Bakery : public Building, public MoneyProducer
{
public:
    Bakery(int tx, int ty);

    static void loadTexture();

    Cost cost() const override { return {Currency::Money, 40}; }
    BuildingType type() const override { return BuildingType::Bakery; }
    bool requiresRoadAccess() const override;
    bool canBePlaced(const City &) const override;

    BuildTool buildTool() const override { return BuildTool::PlaceBakery; }

    int moneyPerMinute() const override;
    int storedMoney() const override;
    void setStoredMoney(int v) override;

    void applyOffline(std::int64_t seconds, Inventory &inventory) override;
    void tick(std::int64_t seconds, Inventory &inventory) override;
    int collectMoney();

    void saveTo(PlacedObject &out) const override;
    void loadFrom(const PlacedObject &in) override;

    void render(sf::RenderTarget &target, const sf::Font &font) const override;
    void renderGhost(sf::RenderTarget &target, bool valid) const override;
    void upgrade(const City &city) override;
    bool upgradable() const override { return true; }
    int maxLevel() const override { return 5; }
    Cost upgradeCost() const override;
    int baseStorage() const override { return 50; }
    int storagePerLevel() const override { return 25; }
    int maxStorage() const override
    {
        return baseStorage() + storagePerLevel() * m_level;
    }
    int effectiveMaxLevel(const City &city) const override;

private:
    std::int64_t m_wheatTimer = 0;
    static sf::Texture s_texture;
    int m_storedMoney = 0;
};
