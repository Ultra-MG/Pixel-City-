#pragma once
#include "world/Crop.hpp"
#include "core/Config.hpp"
#include <SFML/Graphics/Texture.hpp>
#include "game/Cost.hpp"
class Carrot : public Crop
{
public:
    static void loadTexture();
    Cost cost() const override { return {Currency::Money, 2}; }
    void render(sf::RenderTarget &target, int x, int y) const override;
    int growthTime() const override { return 12; }
    int value() const override { return 25; }
    const char *typeName() const override { return "carrot"; }

private:
    static sf::Texture s_texture;
};
