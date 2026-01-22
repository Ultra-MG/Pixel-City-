#include "world/City.hpp"

City::City(int w, int h)
    : m_w(w), m_h(h), m_tiles((size_t)w * (size_t)h, Tile::Grass) {}

bool City::inBounds(int x, int y) const
{
  return x >= 0 && y >= 0 && x < m_w && y < m_h;
}

bool City::rectInBounds(int x, int y, int w, int h) const
{
  return w > 0 && h > 0 &&
         inBounds(x, y) &&
         inBounds(x + w - 1, y + h - 1);
}

Tile City::getTile(int x, int y) const
{
  if (!inBounds(x, y))
    return Tile::Grass;
  return m_tiles[(size_t)y * (size_t)m_w + (size_t)x];
}

void City::setTile(int x, int y, Tile t)
{
  if (!inBounds(x, y))
    return;
  m_tiles[(size_t)y * (size_t)m_w + (size_t)x] = t;
}

bool City::rectOverlapsAnyObject(
    int x, int y, int w, int h,
    const Placeable *self) const
{
  for (const auto &o : m_objects)
  {
    if (self && self->canOverlap(*o))
      continue;

    if (x < (o->x + o->w) && (x + w) > o->x &&
        y < (o->y + o->h) && (y + h) > o->y)
      return true;
  }
  return false;
}

bool City::canPlace(const Placeable &obj) const
{
  if (!rectInBounds(obj.x, obj.y, obj.w, obj.h))
    return false;

  if (rectOverlapsAnyObject(obj.x, obj.y, obj.w, obj.h, &obj))
    return false;

  return obj.canBePlaced(*this);
}


void City::place(std::unique_ptr<Placeable> obj)
{
  m_objects.push_back(std::move(obj));
}
