#pragma once
#include "game/SaveTypes.hpp"

class Saveable {
public:
    virtual ~Saveable() = default;

    virtual void saveTo(PlacedObject& out) const = 0;
    virtual void loadFrom(const PlacedObject& in) = 0;
};
