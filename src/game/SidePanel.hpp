#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "ui/PanelButton.hpp"
#include "game/BuildTool.hpp"
#include "game/BuildCatalog.hpp"

class SidePanel {
public:
    SidePanel(float collapsedW, float expandedW, float height);

    void toggle();
    void update(const sf::Vector2f& mouse);
    bool handleClick(const sf::Vector2f& mouse);
    void render(sf::RenderTarget& target);

    BuildTool selectedTool() const { return m_selected; }
    bool isOpen() const { return m_open; }

private:
    enum class Page {
        Categories,
        Entries
    };

    void rebuildButtons();

    float m_collapsedW = 0.f;
    float m_expandedW = 0.f;
    float m_height = 0.f;

    bool m_open = false;

    Page m_page = Page::Categories;
    BuildCategory m_activeCategory = BuildCategory::Infrastructure;
    BuildTool m_selected = BuildTool::None;

    sf::RectangleShape m_bg;
    sf::Font m_font;
    PanelButton m_discardButton;

    std::vector<PanelButton> m_buttons;
};
