#include "world/Infrastructure/CropField.hpp"
#include <SFML/Graphics/Sprite.hpp>

sf::Texture CropField::s_texture;

CropField::CropField(int tx, int ty)
{
    x = tx;
    y = ty;
    w = 1;
    h = 1;
}

void CropField::loadTexture()
{
    s_texture.loadFromFile("assets/infrastructure/cropfield.png");
}

bool CropField::canBePlaced(const City &) const
{
    return true;
}

void CropField::render(sf::RenderTarget &target) const
{
    sf::Sprite s(s_texture);

    s.setPosition(sf::Vector2f{
        float(x * cfg::TileSize),
        float(y * cfg::TileSize)});

    const auto ts = s_texture.getSize();
    if (ts.x > 0 && ts.y > 0) {
        s.setScale(sf::Vector2f{
            float(cfg::TileSize) / ts.x,
            float(cfg::TileSize) / ts.y});
    }

    target.draw(s);

    if (m_crop) {
        m_crop->render(target, x, y);
    }
}

void CropField::renderGhost(sf::RenderTarget &target, bool valid) const
{
    sf::Sprite s(s_texture);

    s.setPosition(sf::Vector2f{
        float(x * cfg::TileSize),
        float(y * cfg::TileSize)});

    const auto ts = s_texture.getSize();
    if (ts.x > 0 && ts.y > 0) {
        s.setScale(sf::Vector2f{
            float(cfg::TileSize) / ts.x,
            float(cfg::TileSize) / ts.y});
    }

    s.setColor(valid
                   ? sf::Color(200, 255, 200, 180)
                   : sf::Color(255, 0, 0, 180));

    target.draw(s);
}

void CropField::plantCrop(Crop* crop) {
    if (!m_crop) {
        m_crop = crop;
    }
}

void CropField::harvest() {
    if (m_crop) {
        delete m_crop;
        m_crop = nullptr;
    }
}