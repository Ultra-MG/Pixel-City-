#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <optional>
#include "ui/PanelButton.hpp"
#include "game/Inventory.hpp"

class InventoryPanel
{
public:
    InventoryPanel(float collapsedW, float expandedW, float height);

    void toggle();
    bool isOpen() const { return m_open; }
    void refresh(const Inventory &inv);
    std::optional<std::string> handleClick(const sf::Vector2f &mouse);
    void render(sf::RenderTarget &target);

private:
    void initInventoryPanel();
    void rebuild(const Inventory &inv);
    float m_collapsedW = 0.f;
    float m_expandedW = 0.f;
    float m_height = 0.f;
    bool m_open = false;

    sf::RectangleShape m_bg;
    sf::Font m_font;
    std::unordered_map<std::string, sf::Texture> m_textures;
    struct Row
    {
        std::string id;
        PanelButton button;
    };
    std::vector<Row> m_rows;
};
