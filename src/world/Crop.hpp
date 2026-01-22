#pragma once
#include <SFML/Graphics/RenderTarget.hpp>

class Crop {
public:
    virtual ~Crop() = default;
    virtual void render(sf::RenderTarget& target, int x, int y) const = 0;
    virtual int growthTime() const = 0;
    virtual int value() const = 0;
};