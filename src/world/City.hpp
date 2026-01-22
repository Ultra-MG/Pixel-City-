#pragma once

#include <vector>
#include <memory>
#include "world/Tile.hpp"
#include "world/Placeable.hpp"

class City
{
public:
  City(int w, int h);

  int w() const { return m_w; }
  int h() const { return m_h; }

  Tile getTile(int x, int y) const;
  void setTile(int x, int y, Tile t);
  bool isWater(int tx, int ty) const;

  // GENERIC OBJECT ACCESS
  const std::vector<std::unique_ptr<Placeable>> &objects() const
  {
    return m_objects;
  }
  bool canPlace(const Placeable &obj) const;
  void place(std::unique_ptr<Placeable> obj);
  Placeable* getPlaceableAt(int tx, int ty) const;

private:
  int m_w = 0;
  int m_h = 0;
  std::vector<Tile> m_tiles;
  std::vector<std::unique_ptr<Placeable>> m_objects;

  bool inBounds(int x, int y) const;
  bool rectInBounds(int x, int y, int w, int h) const;
  bool rectOverlapsAnyObject(
      int x, int y, int w, int h,
      const Placeable *self) const;
};
