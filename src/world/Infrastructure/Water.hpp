#pragma once

#include "world/Infrastructure.hpp"
#include "core/Config.hpp"
#include "world/City.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "game/Cost.hpp"

class Water : public Infrastructure
{
public:
    Water(int tx, int ty);

    static void loadTexture();

    Cost cost() const override { return {Currency::Money, 2}; }
    bool canBePlaced(const City &) const override;

    void render(sf::RenderTarget &target, const sf::Font& font) const override;
    void renderGhost(sf::RenderTarget &target, bool valid) const override;
    bool canOverlap(const Placeable &) const override { return false; }
    BuildTool buildTool() const override
    {
        return BuildTool::PlaceWater;
    }

private:
    static sf::Texture s_texture;
};
