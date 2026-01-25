#pragma once
#include "world/Placeable.hpp"
#include <cstdint>
enum class BuildingType : std::uint8_t
{
  House,
  TownHall,
  Farm,
  Store,
  Bakery
};

class Building : public Placeable
{
public:
  virtual BuildingType type() const = 0;
  virtual bool requiresRoadAccess() const { return false; }
};
