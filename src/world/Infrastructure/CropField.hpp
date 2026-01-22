#pragma once

#include "world/Placeable.hpp"
#include "core/Config.hpp"
#include "world/City.hpp"
#include "world/Infrastructure.hpp"
#include "world/Crop.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class CropField : public Infrastructure {
public:
    CropField(int tx, int ty);

    static void loadTexture();

    int cost() const override { return 50; }
    bool canBePlaced(const City&) const override;

    void render(sf::RenderTarget& target) const override;
    void renderGhost(sf::RenderTarget& target, bool valid) const override;

    void plantCrop(Crop* crop);
    void harvest();
    const Crop* crop() const { return m_crop; }

private:
    static sf::Texture s_texture;
    Crop* m_crop = nullptr;
};
