#include "ui/PanelButton.hpp"
#include <algorithm>

void PanelButton::setText(const sf::Font& font,
                          const std::string& str,
                          unsigned size , sf::Color color)
{
    m_text.emplace(font, str, size);
    m_text->setFillColor(color);
}

void PanelButton::setTextOffset(sf::Vector2f offset)
{
    m_textOffset = offset;
}

void PanelButton::setCost(const sf::Font &font, const std::string &amount, const sf::Texture *icon)
{
    if (amount.empty() || !icon)
    {
        m_costText.reset();
        m_costSprite.reset();
        return;
    }

    m_costText.emplace(font, amount, 7);
    m_costText->setFillColor(sf::Color::Black);
    m_costSprite.emplace(*icon);

    const auto size = icon->getSize();
    if (size.x > 0 && size.y > 0)
    {
        const float scale = m_costIconSize / static_cast<float>(std::max(size.x, size.y));
        m_costSprite->setScale({scale, scale});
    }
}

void PanelButton::draw(sf::RenderTarget& target)
{
    Button::draw(target);

    if (m_text)
    {
        sf::Text t = *m_text;
        t.setPosition(m_bg.getPosition() + m_textOffset);
        target.draw(t);
    }

    if (m_costText && m_costSprite)
    {
        const sf::Vector2f pos = m_bg.getPosition();
        const sf::Vector2f size = m_bg.getSize();

        const sf::FloatRect costBounds = m_costText->getLocalBounds();
        const float textW = costBounds.size.x;
        const float textH = costBounds.size.y;

        const float right = pos.x + size.x - m_costPadding;
        const float iconX = right - m_costIconSize;
        const float textX = iconX - m_costPadding - textW;
        const float centerY = pos.y + (size.y - textH) * 0.5f - costBounds.position.y;
        const float iconY = pos.y + (size.y - m_costIconSize) * 0.5f;

        sf::Sprite s = *m_costSprite;
        s.setPosition({iconX, iconY});
        target.draw(s);

        sf::Text t = *m_costText;
        t.setPosition({textX, centerY});
        target.draw(t);
    }
}
