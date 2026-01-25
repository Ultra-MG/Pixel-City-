#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include "game/Cost.hpp"
#include <unordered_map>
#include <string>

class Crop
{
public:
    virtual ~Crop() = default;

    virtual Cost cost() const = 0;
    virtual void render(sf::RenderTarget &target, int x, int y) const = 0;
    virtual int growthTime() const = 0;
    virtual int value() const = 0;
    virtual const char *typeName() const = 0;
    virtual const char *iconPath() const = 0;

    virtual void saveTo(std::unordered_map<std::string, std::string>& out) const
    {
        out["crop_type"] = typeName();
    }
};
