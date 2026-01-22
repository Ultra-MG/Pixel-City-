// world/TownHall.hpp
#pragma once

#include "world/Building.hpp"
#include "core/Config.hpp"
#include "world/City.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class TownHall : public Building {
public:
    TownHall(int tx, int ty);

    static void loadTexture();

    BuildingType type() const override;
    bool requiresRoadAccess() const override;
    bool canBePlaced(const City&) const override;

    void render(sf::RenderTarget& target) const override;
    void renderGhost(sf::RenderTarget& target, bool valid) const override;

private:
    static sf::Texture s_texture;
};
