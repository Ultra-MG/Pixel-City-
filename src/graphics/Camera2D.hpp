#pragma once
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

class Camera2D {
public:
  Camera2D(float w, float h);

  void apply(sf::RenderTarget& target) const;

  // Pan in world units
  void move(sf::Vector2f deltaWorld);

  // factor > 1 zooms IN, factor < 1 zooms OUT
  void zoom(float factor);

  void setCenter(sf::Vector2f worldCenter);

  void setZoomClamp(float minZ, float maxZ);
  float zoomLevel() const { return m_zoom; }

  // screenInternal: mouse position in INTERNAL pixels (0..InternalW/H)
  sf::Vector2f screenToWorld(sf::Vector2f screenInternal) const;

private:
  sf::View m_view;
  sf::Vector2f m_baseSize {0.f, 0.f}; // size at zoom = 1

  float m_zoom = 1.f;     // >1 = zoomed in
  float m_minZoom = 0.5f;
  float m_maxZoom = 3.0f;
};
