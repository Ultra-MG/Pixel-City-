// world/Decorations/Bench.hpp
#pragma once

#include "world/Decoration.hpp"
#include "core/Config.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "game/Cost.hpp"

class Bench : public Decoration {
public:
    Bench(int tx, int ty);

    static void loadTexture();

    Cost cost() const override { return {Currency::Money, 10}; }
    bool canBePlaced(const City&) const override;

    void render(sf::RenderTarget& target,const sf::Font& font) const override;
    void renderGhost(sf::RenderTarget& target, bool valid) const override;
    BuildTool buildTool() const override
    {
        return BuildTool::PlaceBench;
    }

private:
    static sf::Texture s_texture;
};
