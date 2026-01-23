// world/Infrastructure/CropField.hpp  (MODIFIED)
#pragma once
#include "world/Infrastructure.hpp"
#include "world/Crop.hpp"
#include "core/Config.hpp" 
#include <SFML/Graphics/Texture.hpp>

class CropField : public Infrastructure {
public:
    CropField(int tx, int ty);

    static void loadTexture();

    Cost cost() const override { return {Currency::Money, 1}; }
    bool canBePlaced(const City&) const override;

    void render(sf::RenderTarget& target, const sf::Font& font) const override;
    void renderGhost(sf::RenderTarget& target, bool valid) const override;

    void plantCrop(Crop* crop);
    void harvest();
    const Crop* crop() const { return m_crop; }

    BuildTool buildTool() const override
    {
        return BuildTool::PlaceCropField;
    }

    void saveTo(PlacedObject& out) const override;
    void loadFrom(const PlacedObject& in) override;

private:
    static sf::Texture s_texture;
    Crop* m_crop = nullptr;
};
