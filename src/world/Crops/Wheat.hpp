#pragma once
#include "world/Crop.hpp"
#include "core/Config.hpp"
#include <SFML/Graphics/Texture.hpp>
#include "game/Cost.hpp"
class Wheat : public Crop {
public:
    static void loadTexture();
    Cost cost() const override { return {Currency::Money, 1}; }
    void render(sf::RenderTarget& target, int x, int y) const override;
    int growthTime() const override { return 10; }
    int value() const override { return 20; }
    const char* typeName() const override { return "wheat"; }

private:
    static sf::Texture s_texture;
};
