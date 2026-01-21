#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class RadialMenu {
public:
  struct Style {
    float innerRadius = 28.f;
    float outerRadius = 72.f;
    float iconSize    = 24.f;
    int   arcSteps    = 24;

    sf::Color sliceBase      {40, 40, 50, 200};
    sf::Color sliceHover     {80, 80, 100, 220};
    sf::Color outline        {20, 20, 30, 255};

    sf::Color centerFill     {30, 30, 40, 255};
    sf::Color centerOutline  {120, 120, 140, 255};

    sf::Color iconColor      {255, 255, 255, 255};
    sf::Color disabledTint   {140, 140, 140, 255};
  };

  RadialMenu() = default;

  void setStyle(const Style& s) { m_style = s; }
  const sf::Vector2f& center() const { return m_center; }
  sf::Vector2f& center() { return m_center; }

  const Style& style() const { return m_style; }
  Style& style() { return m_style; }

  void setCenter(const sf::Vector2f& c) { m_center = c; }
  void setOpen(bool o) { m_open = o; }
  bool open() const { return m_open; }

  void clearItems();
  void addItem(int id, const std::string& iconPath, bool enabled);

  int  hoverIndex(const sf::Vector2f& mouse) const;
  int  click(const sf::Vector2f& mouse) const;

  void draw(sf::RenderTarget& target, const sf::Vector2f& mouse) const;

private:
  struct Item {
    int id;
    bool enabled;
    sf::Texture texture;
    mutable sf::Sprite sprite;

    Item(int i, bool e, sf::Texture&& t)
    : id(i), enabled(e), texture(std::move(t)), sprite(texture) {}
  };


  std::vector<Item> m_items;

  bool m_open = false;
  sf::Vector2f m_center {0.f, 0.f};
  Style m_style;

  float dist(sf::Vector2f a, sf::Vector2f b) const;
  float angleUpCW(sf::Vector2f v) const;
  int   indexFromAngle(float ang) const;

  void drawSlice(sf::RenderTarget& target, int idx, bool hovered) const;
  void drawCenter(sf::RenderTarget& target) const;
  void drawIcons(sf::RenderTarget& target) const;
};
