#pragma once
#include <SFML/Graphics.hpp>
#include "world/Saveable.hpp"
#include "game/SaveTypes.hpp"

class WorldObject : public Saveable {
public:
  virtual ~WorldObject() = default;

  virtual void render(sf::RenderTarget& target, const sf::Font& font) const = 0;
  virtual void update(float) {}
  void saveTo(PlacedObject& out) const override;
  void loadFrom(const PlacedObject& in) override;

  int x = 0, y = 0;  
  int w = 1, h = 1;   
};
