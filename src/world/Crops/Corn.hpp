#pragma once
#include "world/Crop.hpp"
#include "core/Config.hpp"
#include <SFML/Graphics/Texture.hpp>
#include "game/Cost.hpp"
class Corn : public Crop {
public:
    static void loadTexture();
    
    Cost cost() const override { return {Currency::Money, 3}; }
    void render(sf::RenderTarget& target, int x, int y) const override;
    int growthTime() const override { return 15; }
    int value() const override { return 30; }
    const char* typeName() const override { return "corn"; }

private:
    static sf::Texture s_texture;
};
