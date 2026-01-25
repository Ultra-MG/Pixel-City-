#pragma once
#include "world/Crop.hpp"
#include "core/Config.hpp"
#include <SFML/Graphics/Texture.hpp>
#include "game/Cost.hpp"
class Potato : public Crop {
public:
    static void loadTexture();
    Cost cost() const override { return {Currency::Money, 3}; }
    void render(sf::RenderTarget& target, int x, int y) const override;
    int growthTime() const override { return 14; }
    int value() const override { return 28; }
    const char* typeName() const override { return "potato"; }
    const char* iconPath() const override { return "assets/crops/potato.png"; }

private:
    static sf::Texture s_texture;
};
