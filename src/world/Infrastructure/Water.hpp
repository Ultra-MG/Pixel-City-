// world/Infrastructure/Water.hpp
#pragma once

#include "world/Infrastructure.hpp"
#include "core/Config.hpp"
#include "world/City.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class Water : public Infrastructure
{
public:
    Water(int tx, int ty);

    static void loadTexture();

    int cost() const override { return 0; }
    bool canBePlaced(const City &) const override;

    void render(sf::RenderTarget &target) const override;
    void renderGhost(sf::RenderTarget &target, bool valid) const override;
    bool canOverlap(const Placeable &) const override { return false; }

private:
    static sf::Texture s_texture;
};
