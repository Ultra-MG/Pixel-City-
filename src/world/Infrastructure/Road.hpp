// world/Road.hpp
#pragma once

#include "world/Placeable.hpp"
#include "core/Config.hpp"
#include "world/City.hpp"
#include "world/Infrastructure.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "game/Cost.hpp"

class Road : public Infrastructure
{
public:
    Road(int tx, int ty);

    static void loadTexture();

    Cost cost() const override { return {Currency::Money, 1}; }
    bool canBePlaced(const City &) const override;

    void render(sf::RenderTarget &target) const override;
    void renderGhost(sf::RenderTarget &target, bool valid) const override;
    BuildTool buildTool() const override
    {
        return BuildTool::PlaceRoad;
    }

private:
    static sf::Texture s_texture;
};
