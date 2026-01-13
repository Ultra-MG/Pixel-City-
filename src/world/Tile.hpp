#pragma once
#include <cstdint>

enum class Tile : std::uint8_t {
  Grass = 0,
  Road  = 1
};

inline bool isRoad(Tile t) { return t == Tile::Road; }
