#pragma once
#include <SFML/Graphics.hpp>
#include "world/Saveable.hpp"
#include "game/SaveTypes.hpp"
#include "game/Cost.hpp"
class City;
class WorldObject : public Saveable
{
public:
  virtual ~WorldObject() = default;

  virtual void render(sf::RenderTarget &target, const sf::Font &font) const = 0;
  virtual void update(float) {}
  void saveTo(PlacedObject &out) const override;
  void loadFrom(const PlacedObject &in) override;
  virtual bool upgradable() const { return false; }
  virtual int maxLevel() const { return 1; }
  virtual Cost upgradeCost() const { return Cost(Currency::Money, 0); }
  virtual void upgrade(const City &city) {}
  virtual int effectiveMaxLevel(const City &city) const
  {
    return maxLevel();
  }
  virtual bool canUpgrade(const City &city) const
  {
    return (upgradable() && level() < effectiveMaxLevel(city));
  }

  int level() const { return m_level; }
  int m_level = 1;
  int x = 0, y = 0;
  int w = 1, h = 1;
};

void drawLevelBadge(const WorldObject &obj,
                    sf::RenderTarget &target,
                    const sf::Font &font);

void drawCoinBadge(const WorldObject &obj, sf::RenderTarget &target,
                   const sf::Font &font, const int m_storedMoney);