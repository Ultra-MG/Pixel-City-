#pragma once
#include "ui/Button.hpp"
#include <SFML/Graphics/Text.hpp>
#include "game/BuildCatalog.hpp"
#include "game/BuildTool.hpp"

class PanelButton : public Button
{
public:
    using Button::Button; // reuse constructors

    void setText(const sf::Font &font, const std::string &str, unsigned size , sf::Color color);
    void setTextOffset(sf::Vector2f offset);
    void setCost(const sf::Font &font, const std::string &amount, const sf::Texture *icon);

    void draw(sf::RenderTarget &target);

    BuildCategory category = BuildCategory::Infrastructure;
    BuildTool tool = BuildTool::None;
    bool isBack = false;

private:
    std::optional<sf::Text> m_text;
    sf::Vector2f m_textOffset{0.f, 0.f};
    std::optional<sf::Text> m_costText;
    std::optional<sf::Sprite> m_costSprite;
    float m_costIconSize = 10.f;
    float m_costPadding = 4.f;
};
