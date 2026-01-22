// world/Infrastructure/CropField.cpp  (MODIFIED)
#include "world/Infrastructure/CropField.hpp"
#include "world/CropFactory.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

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
    s.setPosition({float(x * cfg::TileSize), float(y * cfg::TileSize)});
    s.setScale({
        float(cfg::TileSize) / s_texture.getSize().x,
        float(cfg::TileSize) / s_texture.getSize().y
    });
    target.draw(s);

    if (m_crop)
        m_crop->render(target, x, y);
}

void CropField::renderGhost(sf::RenderTarget &target, bool valid) const
{
    sf::Sprite s(s_texture);
    s.setPosition({float(x * cfg::TileSize), float(y * cfg::TileSize)});
    s.setScale({
        float(cfg::TileSize) / s_texture.getSize().x,
        float(cfg::TileSize) / s_texture.getSize().y
    });
    s.setColor(valid ? sf::Color(200,255,200,180)
                     : sf::Color(255,0,0,180));
    target.draw(s);
}

void CropField::plantCrop(Crop* crop)
{
    if (!m_crop)
        m_crop = crop;
}

void CropField::harvest()
{
    delete m_crop;
    m_crop = nullptr;
}

void CropField::saveTo(PlacedObject& out) const
{
    Infrastructure::saveTo(out);

    if (m_crop)
        m_crop->saveTo(out.data);
    std::cout<<"Saved crop field with crop type: "
             << (m_crop ? m_crop->typeName() : "none") << "\n";
}

void CropField::loadFrom(const PlacedObject& in)
{
    Infrastructure::loadFrom(in);

    auto it = in.data.find("crop_type");
    if (it == in.data.end())
        return;

    m_crop = CropFactory::create(it->second);
    std::cout<<"Loaded crop field with crop type: "
             << (m_crop ? m_crop->typeName() : "none") << "\n";
}
