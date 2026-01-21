#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

class Button
{
public:
    Button();
    Button(sf::Vector2f size, sf::Vector2f pos);

    Button(const Button &) = delete;
    Button &operator=(const Button &) = delete;

    Button(Button &&) = default;
    Button &operator=(Button &&) = default;

    bool loadImage(const std::string &path);
    void setImageScale(sf::Vector2f scale);
    void setImageOffset(sf::Vector2f offset);
    void setImageSize(sf::Vector2f size);
    void setColor(sf::Color c);

    void setOutline(sf::Color c, float t);

    bool contains(sf::Vector2f p) const;
    void draw(sf::RenderTarget &target);

protected:
    sf::RectangleShape m_bg;
    sf::Texture m_texture;
    std::optional<sf::Sprite> m_sprite;
    sf::Vector2f m_imageOffset{0.f, 0.f};
};
