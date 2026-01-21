#include "ui/Button.hpp"

Button::Button()
    : m_bg(sf::Vector2f{0.f, 0.f})
{
}

// Button::Button(sf::Vector2f size, sf::Vector2f pos)
//     : m_bg(size)
// {
//     m_bg.setPosition(pos);
// }

Button::Button(sf::Vector2f size, sf::Vector2f pos)
{
    m_bg.setSize(size);
    m_bg.setPosition(pos);
}

void Button::setColor(sf::Color c)
{
    m_bg.setFillColor(c);
}

void Button::setImageSize(sf::Vector2f size)
{
    if (!m_sprite)
        return;

    const sf::Vector2u texSize = m_texture.getSize();
    if (texSize.x == 0 || texSize.y == 0)
        return;

    m_sprite->setScale(sf::Vector2f{
        size.x / static_cast<float>(texSize.x),
        size.y / static_cast<float>(texSize.y)});
}

bool Button::loadImage(const std::string &path)
{
    if (!m_texture.loadFromFile(path))
        return false;

    m_texture.setSmooth(false);
    m_sprite.emplace(m_texture);
    m_sprite->setTextureRect(
        sf::IntRect({0, 0},
                    sf::Vector2i(m_texture.getSize())));
    m_sprite->setColor(sf::Color::White);
    return true;
}

void Button::setImageScale(sf::Vector2f scale)
{
    if (m_sprite)
        m_sprite->setScale(scale);
}

void Button::setImageOffset(sf::Vector2f offset)
{
    m_imageOffset = offset;
}

void Button::setOutline(sf::Color c, float t)
{
    m_bg.setOutlineColor(c);
    m_bg.setOutlineThickness(t);
}

bool Button::contains(sf::Vector2f p) const
{
    return m_bg.getGlobalBounds().contains(p);
}

void Button::draw(sf::RenderTarget &target)
{
    target.draw(m_bg);

    if (m_sprite)
    {
        m_sprite->setPosition(m_bg.getPosition() + m_imageOffset);
        target.draw(*m_sprite);
    }
}
