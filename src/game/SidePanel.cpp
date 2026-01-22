#include "game/SidePanel.hpp"
#include "game/Economy.hpp"
#include <string>
#include <stdexcept>

SidePanel::SidePanel(float collapsedW, float expandedW, float height)
    : m_collapsedW(collapsedW), m_expandedW(expandedW), m_height(height)
{
    m_bg.setPosition({0.f, 0.f});
    m_bg.setSize({0.f, 0.f});
    m_bg.setFillColor(sf::Color(30, 30, 30, 220));

    if (!m_font.openFromFile("assets/fonts/pixelFont.ttf"))
    {
        throw std::runtime_error("SidePanel: failed to load font");
    }

    m_coinTexture.emplace();
    m_diamondTexture.emplace();
    if (!m_coinTexture->loadFromFile("assets/coin.png"))
    {
        throw std::runtime_error("SidePanel: failed to load assets/coin.png");
    }
    if (!m_diamondTexture->loadFromFile("assets/diamond.png"))
    {
        throw std::runtime_error("SidePanel: failed to load assets/diamond.png");
    }
    m_coinTexture->setSmooth(false);
    m_diamondTexture->setSmooth(false);
    m_discardButton = PanelButton(sf::Vector2f{24.f, 24.f}, sf::Vector2f{expandedW - 34.f, 6.f});
    m_discardButton.setOutline(sf::Color(160, 80, 80), 1.f);
    m_discardButton.loadImage("assets/ui/discard.png");
    m_discardButton.setImageSize({16.f, 16.f});
    m_discardButton.setImageOffset({4.f, 4.f});

    rebuildButtons();
}

void SidePanel::toggle()
{
    m_open = !m_open;
    rebuildButtons();
}

void SidePanel::update(const sf::Vector2f &) {}

void SidePanel::rebuildButtons()
{
    m_buttons.clear();

    if (m_page == Page::Categories)
        m_buttons.reserve(3);
    else
        m_buttons.reserve(buildCatalog().size() + 1);

    if (!m_open)
    {
        m_bg.setSize({0.f, 0.f});
        return;
    }

    m_bg.setSize({m_expandedW, m_height});

    float y = 40.f;

    if (m_page == Page::Entries)
    {

        m_buttons.emplace_back(sf::Vector2f{24.f, 24.f}, sf::Vector2f{120.f, 150.f});
        auto &b = m_buttons.back();
        b.isBack = true;
        b.category = BuildCategory::Infrastructure;
        b.setOutline(sf::Color(120, 80, 85), 1.f);
        b.setText(m_font, "Back", 7, sf::Color::Black);
        b.setTextOffset({4.f, 6.f});
        y += 30.f;
    }

    if (m_page == Page::Categories)
    {

        {

            m_buttons.emplace_back(sf::Vector2f{120.f, 24.f}, sf::Vector2f{10.f, y});
            auto &b = m_buttons.back();

            b.category = BuildCategory::Building;
            b.setOutline(sf::Color(120, 80, 85), 1.f);
            b.loadImage("assets/categories/building.png");
            b.setImageSize({20.f, 20.f});
            b.setImageOffset({4.f, 2.f});
            b.setText(m_font, "Building", 7, sf::Color::Black);
            b.setTextOffset({28.f, 8.f});

            y += 30.f;
        }

        {
            m_buttons.emplace_back(sf::Vector2f{120.f, 24.f}, sf::Vector2f{10.f, y});
            auto &b = m_buttons.back();

            b.category = BuildCategory::Infrastructure;
            b.setOutline(sf::Color(120, 80, 85), 1.f);
            b.loadImage("assets/categories/infrastructure.png");
            b.setImageSize({20.f, 20.f});
            b.setImageOffset({4.f, 2.f});
            b.setText(m_font, "Infrastructure", 7, sf::Color::Black);
            b.setTextOffset({28.f, 8.f});

            y += 30.f;
        }

        {

            m_buttons.emplace_back(sf::Vector2f{120.f, 24.f}, sf::Vector2f{10.f, y});
            auto &b = m_buttons.back();

            b.category = BuildCategory::Decorations;
            b.setOutline(sf::Color(120, 80, 85), 1.f);
            b.loadImage("assets/categories/decorations.png");
            b.setImageSize({20.f, 20.f});
            b.setImageOffset({4.f, 2.f});
            b.setText(m_font, "Decoration", 7, sf::Color::Black);
            b.setTextOffset({28.f, 8.f});

            y += 30.f;
        }

        return;
    }
    y = 40.f;

    for (const auto &entry : buildCatalog())
    {
        if (entry.category != m_activeCategory)
            continue;

        m_buttons.emplace_back(sf::Vector2f{120.f, 24.f}, sf::Vector2f{10.f, y});
        auto &b = m_buttons.back();

        b.tool = entry.tool;
        b.category = entry.category;
        b.setOutline(sf::Color(120, 80, 85), 1.f);
        b.loadImage(entry.iconPath);
        b.setImageSize({20.f, 20.f});
        b.setImageOffset({4.f, 2.f});
        b.setText(m_font, entry.label, 7, sf::Color::Black);
        b.setTextOffset({28.f, 8.f});

        const Cost cost = buildCost(entry.tool);
        if (cost.amount > 0)
        {
            const sf::Texture *icon =
                (cost.currency == Currency::Money) ? &(*m_coinTexture) : &(*m_diamondTexture);
            b.setCost(m_font, std::to_string(cost.amount), icon);
        }

        y += 30.f;
    }
}

bool SidePanel::handleClick(const sf::Vector2f &mouse)
{

    if (!m_open)
        return false;

    if (m_open && m_discardButton.contains(mouse))
    {
        m_selected = BuildTool::None;
        return true;
    }

    for (auto &b : m_buttons)
    {
        if (!b.contains(mouse))
            continue;

        if (b.isBack)
        {
            m_page = Page::Categories;
            rebuildButtons();
            return true;
        }

        if (m_page == Page::Categories)
        {
            m_activeCategory = b.category;
            m_page = Page::Entries;
            rebuildButtons();
            return true;
        }

        m_selected = b.tool;
        m_open = false;
        rebuildButtons();
        return true;
    }

    return false;
}

void SidePanel::render(sf::RenderTarget &target)
{
    if (!m_open)
        return;

    target.draw(m_bg);

    for (auto &b : m_buttons)
    {
        b.draw(target);
    }

    m_discardButton.draw(target);
}
