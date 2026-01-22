#pragma once
#include "world/Crop.hpp"
#include "core/Config.hpp"
#include <SFML/Graphics/Texture.hpp>

class Corn : public Crop {
public:
    static void loadTexture();
    void render(sf::RenderTarget& target, int x, int y) const override;
    int growthTime() const override { return 15; }
    int value() const override { return 30; }

private:
    static sf::Texture s_texture;
};