#include "game/InventoryPanel.hpp"
#include "world/CropRegistry.hpp"
#include <stdexcept>

InventoryPanel::InventoryPanel(float collapsedW, float expandedW, float height)
    : m_collapsedW(collapsedW), m_expandedW(expandedW), m_height(height)
{
    m_bg.setPosition({0.f, 0.f});
    m_bg.setSize({0.f, 0.f});
    m_bg.setFillColor(sf::Color(30, 30, 30, 220));

    if (!m_font.openFromFile("assets/fonts/pixelFont.ttf"))
        throw std::runtime_error("InventoryPanel: failed to load font");
    m_font.setSmooth(false);

    for (const auto &crop : CropRegistry::all())
    {
        sf::Texture tex;
        if (tex.loadFromFile(crop.iconPath))
        {
            tex.setSmooth(false);
            m_textures.emplace(crop.id, std::move(tex));
        }
    }
}

void InventoryPanel::toggle()
{
    m_open = !m_open;
}

void InventoryPanel::refresh(const Inventory &inv)
{
    if (!m_open)
        return;
    rebuild(inv);
}

std::optional<std::string> InventoryPanel::handleClick(const sf::Vector2f &mouse)
{
    if (!m_open)
        return std::nullopt;
    for (auto &row : m_rows)
    {
        if (row.button.contains(mouse))
            return row.id;
    }
    return std::nullopt;
}

void InventoryPanel::rebuild(const Inventory &inv)
{
    m_rows.clear();
    if (!m_open)
    {
        m_bg.setSize({0.f, 0.f});
        return;
    }

    m_bg.setSize({m_expandedW, m_height});
    float y = 40.f;

    for (const auto &crop : CropRegistry::all())
    {
        const int count = inv.count(crop.id);
        Row row;
        row.id = crop.id;
        row.button = PanelButton(sf::Vector2f{120.f, 24.f}, sf::Vector2f{10.f, y});
        auto &b = row.button;
        b.setOutline(sf::Color(120, 80, 85), 1.f);
        b.loadImage(crop.iconPath);
        b.setImageSize({20.f, 20.f});
        b.setImageOffset({4.f, 2.f});
        b.setText(m_font, crop.label, 7, sf::Color::Black);
        b.setTextOffset({28.f, 8.f});

        auto it = m_textures.find(crop.id);
        if (it != m_textures.end())
        {
            b.setCost(m_font, std::to_string(count), &it->second);
        }
        m_rows.push_back(std::move(row));
        y += 30.f;
    }
}

void InventoryPanel::render(sf::RenderTarget &target)
{
    if (!m_open)
        return;

    target.draw(m_bg);
    for (auto &row : m_rows)
        row.button.draw(target);
}
