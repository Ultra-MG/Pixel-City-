#include "world/City.hpp"
#include "world/Building.hpp"
#include "world/Infrastructure/Road.hpp"
#include "world/Infrastructure/Water.hpp"
#include <algorithm>
#include "world/WorldObject.hpp"
#include "world/Buildings/TownHall.hpp"

City::City(int w, int h)
    : m_w(w), m_h(h), m_tiles((size_t)w * (size_t)h, Tile::Grass) {}

bool City::inBounds(int x, int y) const
{
  return x >= 0 && y >= 0 && x < m_w && y < m_h;
}

bool City::isWater(int tx, int ty) const
{
    for (const auto& obj : m_objects)
    {
        if (obj->x == tx && obj->y == ty)
        {
            return dynamic_cast<const Water*>(obj.get()) != nullptr;
        }
    }
    return false;
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

TownHall* City::townHall() const
{
    for (const auto& o : m_objects)
        if (auto* th = dynamic_cast<TownHall*>(o.get()))
            return th;
    return nullptr;
}

int City::townHallLevel() const
{
    if (auto* th = townHall())
        return th->level();
    return 1;
}


bool City::canPlace(const Placeable &obj) const
{
  if (!rectInBounds(obj.x, obj.y, obj.w, obj.h))
    return false;

  if (rectOverlapsAnyObject(obj.x, obj.y, obj.w, obj.h, &obj))
    return false;

  if (const auto *b = dynamic_cast<const Building *>(&obj))
  {
    if (b->requiresRoadAccess())
    {
      if (!hasRoadAdjacent(obj.x, obj.y, obj.w, obj.h))
        return false;
    }
  }

  return obj.canBePlaced(*this);
}

bool City::hasRoadAdjacent(int x, int y, int w, int h) const
{
  const int left = x - 1;
  const int right = x + w;
  const int top = y - 1;
  const int bottom = y + h;

  for (const auto &o : m_objects)
  {
    if (!dynamic_cast<Road *>(o.get()))
      continue;

    for (int ry = o->y; ry < o->y + o->h; ++ry)
    {
      for (int rx = o->x; rx < o->x + o->w; ++rx)
      {
        if (rx >= x && rx < x + w && (ry == top || ry == bottom))
          return true;
        if (ry >= y && ry < y + h && (rx == left || rx == right))
          return true;
      }
    }
  }

  return false;
}

bool City::removePlaceable(const Placeable *obj)
{
  if (!obj)
    return false;
  if (!obj->canBeDeleted())
    return false;

  auto it = std::find_if(m_objects.begin(), m_objects.end(),
                         [obj](const std::unique_ptr<Placeable> &p)
                         { return p.get() == obj; });
  if (it == m_objects.end())
    return false;
  (*it)->onDeleted();
  m_objects.erase(it);
  return true;
}


void City::place(std::unique_ptr<Placeable> obj)
{
  m_objects.push_back(std::move(obj));
}

Placeable* City::getPlaceableAt(int tx, int ty) const {
    for (const auto& o : m_objects) {
        if (o->x <= tx && tx < o->x + o->w && o->y <= ty && ty < o->y + o->h) {
            return o.get();
        }
    }
    return nullptr;
}
