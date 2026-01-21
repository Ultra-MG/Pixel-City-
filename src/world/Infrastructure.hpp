#pragma once
#include "world/Placeable.hpp"

class Infrastructure : public Placeable {
public:
    virtual ~Infrastructure() = default;

    virtual int cost() const = 0;
};
