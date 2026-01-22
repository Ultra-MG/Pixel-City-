#pragma once
#include "world/WorldObject.hpp"

class City;

class Placeable : public WorldObject
{
public:
  int x = 0;
  int y = 0;
  int w = 1;
  int h = 1;

  int cost = 0;

  virtual bool canBePlaced(const City &) const = 0;
  virtual void renderGhost(sf::RenderTarget &, bool valid) const = 0;
  virtual bool canOverlap(const Placeable &other) const { return false; }
  virtual ~Placeable() = default;
};
