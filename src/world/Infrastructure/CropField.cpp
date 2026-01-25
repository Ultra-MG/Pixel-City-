#include "world/Infrastructure/CropField.hpp"
#include "world/CropFactory.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>
#include <string>
#include "game/Wallet.hpp"

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

void CropField::render(sf::RenderTarget &target, const sf::Font &font) const
{
    sf::Sprite s(s_texture);
    s.setPosition({float(x * cfg::TileSize), float(y * cfg::TileSize)});
    s.setScale({float(cfg::TileSize) / s_texture.getSize().x,
                float(cfg::TileSize) / s_texture.getSize().y});
    target.draw(s);

    if (m_crop)
        m_crop->render(target, x, y);
}

void CropField::renderGhost(sf::RenderTarget &target, bool valid) const
{
    sf::Sprite s(s_texture);
    s.setPosition({float(x * cfg::TileSize), float(y * cfg::TileSize)});
    s.setScale({float(cfg::TileSize) / s_texture.getSize().x,
                float(cfg::TileSize) / s_texture.getSize().y});
    s.setColor(valid ? sf::Color(200, 255, 200, 180)
                     : sf::Color(255, 0, 0, 180));
    target.draw(s);
}

void CropField::plantCrop(Crop *crop, Wallet& wallet)
{
    if (!m_crop && crop)
    {
        const Cost c = crop->cost();
        if (c.amount > 0)
        {
            if (!wallet.spend(c))
            {
                delete crop;
                return;
            }
        }

        m_crop = crop;
        m_growthSeconds = 0.f;
    }
}

void CropField::harvest()
{
    delete m_crop;
    m_crop = nullptr;
    m_growthSeconds = 0.f;
}

void CropField::update(float dt)
{
    if (!m_crop)
        return;

    m_growthSeconds += dt;
}

bool CropField::canCollect() const
{
    if (!m_crop)
        return false;
    return m_growthSeconds >= static_cast<float>(m_crop->growthTime());
}

CollectResult CropField::collect()
{
    if (!canCollect())
        return {};

    CollectResult r;
    r.itemId = m_crop->typeName();
    r.itemCount = 1;
    harvest();
    return r;
}

std::string CropField::collectIconId() const
{
    if (!m_crop)
        return "";
    return m_crop->typeName();
}

void CropField::saveTo(PlacedObject &out) const
{
    Infrastructure::saveTo(out);

    if (m_crop)
    {
        m_crop->saveTo(out.data);
        out.data["crop_growth"] = std::to_string(static_cast<int>(m_growthSeconds));
    }
    std::cout << "Saved crop field with crop type: "
              << (m_crop ? m_crop->typeName() : "none") << "\n";
}

void CropField::loadFrom(const PlacedObject &in)
{
    Infrastructure::loadFrom(in);

    auto it = in.data.find("crop_type");
    if (it == in.data.end())
        return;

    m_crop = CropFactory::create(it->second);
    auto growIt = in.data.find("crop_growth");
    if (growIt != in.data.end())
        m_growthSeconds = std::stof(growIt->second);
    else
        m_growthSeconds = 0.f;
}
