// world/Decorations/LampPost.hpp
#pragma once

#include "world/Decoration.hpp"
#include "core/Config.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class LampPost : public Decoration {
public:
    LampPost(int tx, int ty);

    static void loadTexture();

    Cost cost() const override { return {Currency::Diamonds, 2}; }
    bool canBePlaced(const City& city) const override;

    void render(sf::RenderTarget& target, const sf::Font& font) const override;
    void renderGhost(sf::RenderTarget& target, bool valid) const override;
    BuildTool buildTool() const override
    {
        return BuildTool::PlaceLampPost;
    }

private:
    static sf::Texture s_texture;
};
