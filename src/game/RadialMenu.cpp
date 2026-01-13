#include "game/RadialMenu.hpp"
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Image.hpp>
#include <cmath>
#include <iostream>

static constexpr float PI = 3.14159265358979323846f;

void RadialMenu::clearItems() {
  m_items.clear();
}

static sf::Texture makePlaceholderTexture(sf::Color c) {
  sf::Image img;
  img.create(16, 16, c);
  for (unsigned x = 0; x < 16; ++x) { img.setPixel(x, 0, sf::Color::Black); img.setPixel(x, 15, sf::Color::Black); }
  for (unsigned y = 0; y < 16; ++y) { img.setPixel(0, y, sf::Color::Black); img.setPixel(15, y, sf::Color::Black); }

  sf::Texture t;
  t.loadFromImage(img);
  t.setSmooth(true);
  return t;
}

void RadialMenu::addItem(int id, const std::string& iconPath, bool enabled) {
  Item it;
  it.id = id;
  it.enabled = enabled;

  if (!it.tex.loadFromFile(iconPath)) {
    std::cerr << "⚠️ RadialMenu icon failed to load: " << iconPath << "\n";
    it.tex = makePlaceholderTexture(enabled ? sf::Color::White : sf::Color(140,140,140));
  } else {
    it.tex.setSmooth(true);
  }

  it.spr.setTexture(it.tex);
  m_items.push_back(std::move(it));
}

float RadialMenu::dist(sf::Vector2f a, sf::Vector2f b) const {
  const float dx = a.x - b.x;
  const float dy = a.y - b.y;
  return std::sqrt(dx*dx + dy*dy);
}

// 0 at top, clockwise [0..2PI)
float RadialMenu::angleUpCW(sf::Vector2f v) const {
  float a = std::atan2(v.x, -v.y); // key trick
  if (a < 0) a += 2.f * PI;
  return a;
}

int RadialMenu::indexFromAngle(float ang) const {
  const int n = (int)m_items.size();
  if (n <= 0) return -1;

  const float slice = (2.f * PI) / (float)n;
  int idx = (int)std::floor(ang / slice);
  if (idx < 0) idx = 0;
  if (idx >= n) idx = n - 1;
  return idx;
}

int RadialMenu::hoverIndex(sf::Vector2f mouse) const {
  if (!m_open) return -1;
  if (m_items.empty()) return -1;

  const float r = dist(mouse, m_center);
  if (r < m_style.innerRadius || r > m_style.outerRadius) return -1;

  const sf::Vector2f v(mouse.x - m_center.x, mouse.y - m_center.y);
  const int idx = indexFromAngle(angleUpCW(v));
  if (idx < 0) return -1;
  if (!m_items[idx].enabled) return -1;
  return idx;
}

int RadialMenu::click(sf::Vector2f mouse) const {
  const int idx = hoverIndex(mouse);
  if (idx < 0) return -1;
  return m_items[idx].id;
}

void RadialMenu::drawSlice(sf::RenderTarget& target, int idx, bool hovered) const {
  const int n = (int)m_items.size();
  if (n <= 0) return;

  const float slice = (2.f * PI) / (float)n;
  const float a0 = (float)idx * slice;
  const float a1 = (float)(idx + 1) * slice;

  const int steps = std::max(6, m_style.arcSteps);

  sf::ConvexShape poly;
  poly.setPointCount((steps + 1) * 2);

  for (int i = 0; i <= steps; ++i) {
    const float t = (float)i / (float)steps;
    const float a = a0 + (a1 - a0) * t;

    const float x = m_center.x + std::sin(a) * m_style.outerRadius;
    const float y = m_center.y - std::cos(a) * m_style.outerRadius;
    poly.setPoint(i, {x, y});
  }

  for (int i = 0; i <= steps; ++i) {
    const float t = (float)i / (float)steps;
    const float a = a1 - (a1 - a0) * t;

    const float x = m_center.x + std::sin(a) * m_style.innerRadius;
    const float y = m_center.y - std::cos(a) * m_style.innerRadius;
    poly.setPoint((steps + 1) + i, {x, y});
  }

  poly.setFillColor(hovered ? m_style.sliceHover : m_style.sliceBase);
  poly.setOutlineThickness(1.f);
  poly.setOutlineColor(m_style.outline);
  target.draw(poly);
}

void RadialMenu::drawCenter(sf::RenderTarget& target) const {
  sf::CircleShape c;
  c.setRadius(m_style.innerRadius);
  c.setOrigin(m_style.innerRadius, m_style.innerRadius);
  c.setPosition(m_center);
  c.setFillColor(m_style.centerFill);
  c.setOutlineThickness(2.f);
  c.setOutlineColor(m_style.centerOutline);
  target.draw(c);
}

void RadialMenu::drawIcons(sf::RenderTarget& target) const {
  const int n = (int)m_items.size();
  if (n <= 0) return;

  const float slice = (2.f * PI) / (float)n;
  const float rMid = (m_style.innerRadius + m_style.outerRadius) * 0.5f;

  for (int i = 0; i < n; ++i) {
    auto& it = m_items[i];
    auto& spr = it.spr;

    const auto ts = spr.getTexture()->getSize();
    if (ts.x == 0 || ts.y == 0) continue;

    const float aMid = ((float)i + 0.5f) * slice;

    const float cx = m_center.x + std::sin(aMid) * rMid;
    const float cy = m_center.y - std::cos(aMid) * rMid;

    const float sx = m_style.iconSize / (float)ts.x;
    const float sy = m_style.iconSize / (float)ts.y;
    const float s = std::min(sx, sy);

    spr.setOrigin((float)ts.x * 0.5f, (float)ts.y * 0.5f);
    spr.setScale(s, s);
    spr.setPosition(cx, cy);
    spr.setColor(it.enabled ? m_style.iconColor : m_style.disabledTint);

    target.draw(spr);
  }
}

void RadialMenu::draw(sf::RenderTarget& target, sf::Vector2f mouse) const {
  if (!m_open) return;
  if (m_items.empty()) return;

  const int hovered = hoverIndex(mouse);

  for (int i = 0; i < (int)m_items.size(); ++i) {
    drawSlice(target, i, i == hovered);
  }

  drawCenter(target);
  drawIcons(target);
}
