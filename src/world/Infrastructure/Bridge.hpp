#pragma once

#include "world/Infrastructure.hpp"
#include "core/Config.hpp"
#include "world/City.hpp"
#include "world/Infrastructure/Water.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "game/Cost.hpp"

class Bridge : public Infrastructure
{
public:
    Bridge(int tx, int ty);

    static void loadTexture();

    Cost cost() const override { return {Currency::Money, 5}; }
    bool canBePlaced(const City &city) const override;

    void render(sf::RenderTarget &target, const sf::Font& font) const override;
    void renderGhost(sf::RenderTarget &target, bool valid) const override;
    bool canOverlap(const Placeable &other) const override
    {
        return dynamic_cast<const Water *>(&other) != nullptr;
    }
    BuildTool buildTool() const override
    {
        return BuildTool::PlaceBridge;
    }

private:
    static sf::Texture s_texture;
};
