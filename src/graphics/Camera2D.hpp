#pragma once

#include <SFML/Graphics.hpp>

class Camera2D {
public:
  Camera2D(float w, float h);

  // Apply camera view to render target
  void apply(sf::RenderTarget& target) const;

  // Pan in world units
  void move(const sf::Vector2f& deltaWorld);

  // factor > 1 zooms IN, factor < 1 zooms OUT
  void zoom(float factor);

  void setCenter(const sf::Vector2f& worldCenter);

  void setZoomClamp(float minZ, float maxZ);
  float zoomLevel() const { return m_zoom; }

  // screenInternal: mouse position in INTERNAL pixels (pixel renderer space)
  sf::Vector2f screenToWorld(const sf::Vector2f& screenInternal) const;
  sf::Vector2f windowToWorld(const sf::RenderWindow& window,
                             const sf::Vector2i& pixel) const;

private:
  sf::View m_view;

  // View size at zoom = 1
  sf::Vector2f m_baseSize {0.f, 0.f};

  float m_zoom = 1.f;      // >1 = zoomed in
  float m_minZoom = 0.5f;
  float m_maxZoom = 3.0f;
};
