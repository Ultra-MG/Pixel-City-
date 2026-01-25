#pragma once

#include <SFML/Graphics.hpp>

class Camera2D {
public:
  Camera2D(float w, float h);

  void apply(sf::RenderTarget& target) const;

  void move(const sf::Vector2f& deltaWorld);

  void zoom(float factor);

  void setCenter(const sf::Vector2f& worldCenter);

  void setZoomClamp(float minZ, float maxZ);
  float zoomLevel() const { return m_zoom; }

  sf::Vector2f screenToWorld(const sf::Vector2f& screenInternal) const;
  sf::Vector2f windowToWorld(const sf::RenderWindow& window,
                             const sf::Vector2i& pixel) const;

private:
  sf::View m_view;

  sf::Vector2f m_baseSize {0.f, 0.f};

  float m_zoom = 1.f;   
  float m_minZoom = 0.5f;
  float m_maxZoom = 3.0f;
};
