#pragma once

#include "world/Placeable.hpp"

class Decoration : public Placeable {
public:
    virtual ~Decoration() = default;
    virtual int cost() const = 0;
};
