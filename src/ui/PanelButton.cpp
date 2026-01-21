#include "ui/PanelButton.hpp"

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

void PanelButton::draw(sf::RenderTarget& target)
{
    Button::draw(target);

    if (m_text)
    {
        sf::Text t = *m_text;
        t.setPosition(m_bg.getPosition() + m_textOffset);
        target.draw(t);
    }
}
