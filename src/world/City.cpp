#include "world/City.hpp"

City::City(int w, int h) : m_w(w), m_h(h), m_tiles((size_t)w * (size_t)h, Tile::Grass) {}

bool City::inBounds(int x, int y) const {
  return x >= 0 && y >= 0 && x < m_w && y < m_h;
}

bool City::rectInBounds(int x, int y, int w, int h) const {
  return w > 0 && h > 0 && inBounds(x, y) && inBounds(x + w - 1, y + h - 1);
}

Tile City::getTile(int x, int y) const {
  if (!inBounds(x, y)) return Tile::Grass;
  return m_tiles[(size_t)y * (size_t)m_w + (size_t)x];
}

void City::setTile(int x, int y, Tile t) {
  if (!inBounds(x, y)) return;
  m_tiles[(size_t)y * (size_t)m_w + (size_t)x] = t;
}

bool City::hasBuildingAt(int tx, int ty) const {
  for (const auto& b : m_buildings) {
    if (tx >= b.x && tx < b.x + b.w && ty >= b.y && ty < b.y + b.h) return true;
  }
  return false;
}

bool City::rectOverlapsBuilding(int x, int y, int w, int h) const {
  for (const auto& b : m_buildings) {
    const bool overlap =
      x < (b.x + b.w) && (x + w) > b.x &&
      y < (b.y + b.h) && (y + h) > b.y;
    if (overlap) return true;
  }
  return false;
}

bool City::rectHasRoad(int x, int y, int w, int h) const {
  for (int yy = y; yy < y + h; ++yy) {
    for (int xx = x; xx < x + w; ++xx) {
      if (getTile(xx, yy) == Tile::Road) return true;
    }
  }
  return false;
}

bool City::canPlaceRoadAt(int tx, int ty) const {
  if (!inBounds(tx, ty)) return false;
  // ❌ road cannot go on top of a building
  if (hasBuildingAt(tx, ty)) return false;
  return true;
}

bool City::canPlaceBuilding(const Building& b) const {
  if (!rectInBounds(b.x, b.y, b.w, b.h)) return false;

  // ❌ building cannot overlap another building
  if (rectOverlapsBuilding(b.x, b.y, b.w, b.h)) return false;

  // ❌ building cannot be placed on top of roads
  if (rectHasRoad(b.x, b.y, b.w, b.h)) return false;

  return true;
}

void City::placeBuilding(const Building& b) {
  m_buildings.push_back(b);
}
