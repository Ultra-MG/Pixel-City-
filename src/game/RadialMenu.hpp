#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <string>

class RadialMenu {
public:
  struct Style {
    float innerRadius = 12.f;
    float outerRadius = 34.f;
    float iconSize    = 14.f;
    int   arcSteps    = 12;

    sf::Color sliceBase     = sf::Color(0, 0, 0, 160);
    sf::Color sliceHover    = sf::Color(255, 255, 255, 35);
    sf::Color outline       = sf::Color(255, 0, 0, 220);

    sf::Color centerFill    = sf::Color(0, 0, 0, 190);
    sf::Color centerOutline = sf::Color(255, 0, 0, 220);

    sf::Color iconColor     = sf::Color::White;
    sf::Color disabledTint  = sf::Color(180, 180, 180, 120);
  };

  struct Item {
    int id = -1;
    bool enabled = true;
    sf::Texture tex;
    mutable sf::Sprite spr;
  };

public:
  void setStyle(const Style& s) { m_style = s; }
  const Style& style() const { return m_style; }

  void setCenter(sf::Vector2f c) { m_center = c; }
  sf::Vector2f center() const { return m_center; }

  void setOpen(bool v) { m_open = v; }
  bool isOpen() const { return m_open; }

  void clearItems();
  void addItem(int id, const std::string& iconPath, bool enabled = true);

  int hoverIndex(sf::Vector2f mouse) const;
  int click(sf::Vector2f mouse) const;

  void draw(sf::RenderTarget& target, sf::Vector2f mouse) const;

private:
  Style m_style;
  sf::Vector2f m_center{0.f, 0.f};
  bool m_open = false;
  std::vector<Item> m_items;

private:
  float dist(sf::Vector2f a, sf::Vector2f b) const;
  float angleUpCW(sf::Vector2f v) const; // 0 at top, clockwise
  int indexFromAngle(float ang) const;

  void drawSlice(sf::RenderTarget& target, int idx, bool hovered) const;
  void drawCenter(sf::RenderTarget& target) const;
  void drawIcons(sf::RenderTarget& target) const;
};
