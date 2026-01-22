// world/Infrastructure/Bridge.hpp
#pragma once

#include "world/Infrastructure.hpp"
#include "core/Config.hpp"
#include "world/City.hpp"
#include "world/Infrastructure/Water.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class Bridge : public Infrastructure
{
public:
    Bridge(int tx, int ty);

    static void loadTexture();

    int cost() const override { return 2; }
    bool canBePlaced(const City &city) const override;

    void render(sf::RenderTarget &target) const override;
    void renderGhost(sf::RenderTarget &target, bool valid) const override;
    bool canOverlap(const Placeable &other) const override
    {
        return dynamic_cast<const Water *>(&other) != nullptr;
    }

private:
    static sf::Texture s_texture;
};
