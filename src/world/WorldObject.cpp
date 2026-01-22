// world/WorldObject.cpp
#include "world/WorldObject.hpp"

void WorldObject::saveTo(PlacedObject &out) const
{
    out.x = x;
    out.y = y;
}

void WorldObject::loadFrom(const PlacedObject &in)
{
    x = in.x;
    y = in.y;
}
