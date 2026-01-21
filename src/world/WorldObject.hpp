#pragma once
#include <SFML/Graphics.hpp>

class WorldObject {
public:
  virtual ~WorldObject() = default;

  virtual void render(sf::RenderTarget& target) const = 0;
  virtual void update(float) {}

  int x = 0, y = 0;   // tile position
  int w = 1, h = 1;   // size in tiles
};
