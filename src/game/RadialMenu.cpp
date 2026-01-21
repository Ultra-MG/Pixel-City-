#include "game/RadialMenu.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>

static constexpr float PI = 3.14159265358979323846f;

void RadialMenu::clearItems() {
  m_items.clear();
}

static sf::Texture makePlaceholderTexture(sf::Color c) {
  sf::Image img;
  img.resize({16, 16}, c);

  for (unsigned x = 0; x < 16; ++x) {
    img.setPixel({x, 0}, sf::Color::Black);
    img.setPixel({x, 15}, sf::Color::Black);
  }
  for (unsigned y = 0; y < 16; ++y) {
    img.setPixel({0, y}, sf::Color::Black);
    img.setPixel({15, y}, sf::Color::Black);
  }

  sf::Texture t;
  (void)t.loadFromImage(img);
  t.setSmooth(true);
  return t;
}

void RadialMenu::addItem(int id, const std::string& iconPath, bool enabled) {
  sf::Texture tex;

  if (!tex.loadFromFile(iconPath)) {
    std::cerr << "RadialMenu icon failed to load: " << iconPath << "\n";
    tex = makePlaceholderTexture(
      enabled ? sf::Color::White : sf::Color(140, 140, 140)
    );
  } else {
    tex.setSmooth(true);
  }

  m_items.emplace_back(id, enabled, std::move(tex));
}

float RadialMenu::dist(sf::Vector2f a, sf::Vector2f b) const {
  const float dx = a.x - b.x;
  const float dy = a.y - b.y;
  return std::sqrt(dx * dx + dy * dy);
}

float RadialMenu::angleUpCW(sf::Vector2f v) const {
  float a = std::atan2(v.x, -v.y);
  if (a < 0.f) a += 2.f * PI;
  return a;
}

int RadialMenu::indexFromAngle(float ang) const {
  const int n = static_cast<int>(m_items.size());
  if (n <= 0) return -1;

  const float slice = (2.f * PI) / static_cast<float>(n);
  return std::clamp(static_cast<int>(ang / slice), 0, n - 1);
}

int RadialMenu::hoverIndex(const sf::Vector2f& mouse) const {
  if (!m_open || m_items.empty()) return -1;

  const float r = dist(mouse, m_center);
  if (r < m_style.innerRadius || r > m_style.outerRadius) return -1;

  const int idx = indexFromAngle(angleUpCW(mouse - m_center));
  if (!m_items[idx].enabled) return -1;
  return idx;
}

int RadialMenu::click(const sf::Vector2f& mouse) const {
  const int idx = hoverIndex(mouse);
  return (idx < 0) ? -1 : m_items[idx].id;
}

void RadialMenu::drawSlice(sf::RenderTarget& target,
                           int idx,
                           bool hovered) const
{
  const int n = static_cast<int>(m_items.size());
  const float slice = (2.f * PI) / static_cast<float>(n);

  const float a0 = idx * slice;
  const float a1 = (idx + 1) * slice;

  const int steps = std::max(6, m_style.arcSteps);
  sf::ConvexShape poly;
  poly.setPointCount((steps + 1) * 2);

  for (int i = 0; i <= steps; ++i) {
    const float a = a0 + (a1 - a0) * (float(i) / steps);
    poly.setPoint(i, {
      m_center.x + std::sin(a) * m_style.outerRadius,
      m_center.y - std::cos(a) * m_style.outerRadius
    });
  }

  for (int i = 0; i <= steps; ++i) {
    const float a = a1 - (a1 - a0) * (float(i) / steps);
    poly.setPoint(steps + 1 + i, {
      m_center.x + std::sin(a) * m_style.innerRadius,
      m_center.y - std::cos(a) * m_style.innerRadius
    });
  }

  poly.setFillColor(hovered ? m_style.sliceHover : m_style.sliceBase);
  poly.setOutlineThickness(1.f);
  poly.setOutlineColor(m_style.outline);
  target.draw(poly);
}

void RadialMenu::drawCenter(sf::RenderTarget& target) const {
  sf::CircleShape c(m_style.innerRadius);
  c.setOrigin({m_style.innerRadius, m_style.innerRadius});
  c.setPosition(m_center);
  c.setFillColor(m_style.centerFill);
  c.setOutlineThickness(2.f);
  c.setOutlineColor(m_style.centerOutline);
  target.draw(c);
}

void RadialMenu::drawIcons(sf::RenderTarget& target) const {
  const int n = static_cast<int>(m_items.size());
  if (n <= 0) return;

  const float slice = (2.f * PI) / static_cast<float>(n);
  const float rMid = (m_style.innerRadius + m_style.outerRadius) * 0.5f;

  for (int i = 0; i < n; ++i) {
    const auto& it = m_items[i];
    auto& spr = it.sprite;

    const auto ts = spr.getTexture().getSize();
    if (ts.x == 0 || ts.y == 0) continue;

    const float a = (i + 0.5f) * slice;

    spr.setOrigin({ts.x * 0.5f, ts.y * 0.5f});
    spr.setScale({
      m_style.iconSize / static_cast<float>(ts.x),
      m_style.iconSize / static_cast<float>(ts.y)
    });
    spr.setPosition({
      m_center.x + std::sin(a) * rMid,
      m_center.y - std::cos(a) * rMid
    });
    spr.setColor(it.enabled ? m_style.iconColor : m_style.disabledTint);

    target.draw(spr);
  }
}

void RadialMenu::draw(sf::RenderTarget& target,
                      const sf::Vector2f& mouse) const
{
  if (!m_open || m_items.empty()) return;

  const int hovered = hoverIndex(mouse);

  for (int i = 0; i < static_cast<int>(m_items.size()); ++i)
    drawSlice(target, i, i == hovered);

  drawCenter(target);
  drawIcons(target);
}
