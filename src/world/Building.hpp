#pragma once
#include <cstdint>

enum class BuildingType : std::uint8_t {
  House = 0
};

struct Building {
  BuildingType type = BuildingType::House;
  int x = 0;     // tile coords (top-left)
  int y = 0;
  int w = 2;     // size in tiles (house 2x2 for example)
  int h = 2;
  int level = 1;
};
