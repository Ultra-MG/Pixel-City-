#pragma once
#include "world/WorldObject.hpp"
#include "game/BuildTool.hpp"
#include "game/SaveTypes.hpp"
#include "game/Cost.hpp"

class City;

class Placeable : public WorldObject
{
public:

  virtual Cost cost() const = 0;
  virtual BuildTool buildTool() const = 0;
  virtual Cost sellValue() const
  {
    Cost c = cost();
    c.amount = c.amount / 2; 
    return c;
  }

  void saveTo(PlacedObject &out) const
  {
    WorldObject::saveTo(out);
    out.tool = buildTool();
  }

  void loadFrom(const PlacedObject &in) override
  {
    WorldObject::loadFrom(in);
  }
  virtual bool canBePlaced(const City &) const = 0;
  virtual void renderGhost(sf::RenderTarget &, bool valid) const = 0;
  virtual bool canOverlap(const Placeable &other) const { return false; }
  virtual bool canBeDeleted() const { return true; }
  virtual void onDeleted() {}
  virtual ~Placeable() = default;
};
