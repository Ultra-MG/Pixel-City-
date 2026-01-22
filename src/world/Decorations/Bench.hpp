// world/Decorations/Bench.hpp
#pragma once

#include "world/Decoration.hpp"
#include "core/Config.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class Bench : public Decoration {
public:
    Bench(int tx, int ty);

    static void loadTexture();

    int cost() const override { return 3; }
    bool canBePlaced(const City&) const override;

    void render(sf::RenderTarget& target) const override;
    void renderGhost(sf::RenderTarget& target, bool valid) const override;

private:
    static sf::Texture s_texture;
};
