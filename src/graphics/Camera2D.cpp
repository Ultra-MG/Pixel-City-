#include "graphics/Camera2D.hpp"
#include <algorithm>

Camera2D::Camera2D(float w, float h)
: m_baseSize{w, h}
{
  m_view.setSize(m_baseSize);
  m_view.setCenter({ w * 0.5f, h * 0.5f });
}

void Camera2D::apply(sf::RenderTarget& target) const {
  target.setView(m_view);
}

void Camera2D::move(const sf::Vector2f& deltaWorld) {
  m_view.move(deltaWorld);
}

void Camera2D::setZoomClamp(float minZ, float maxZ) {
  m_minZoom = minZ;
  m_maxZoom = maxZ;
  m_zoom = std::clamp(m_zoom, m_minZoom, m_maxZoom);
  m_view.setSize(m_baseSize / m_zoom);
}

void Camera2D::zoom(float factor) {
  float newZoom = m_zoom * factor; // factor > 1 = zoom in
  newZoom = std::clamp(newZoom, m_minZoom, m_maxZoom);

  m_zoom = newZoom;
  m_view.setSize(m_baseSize / m_zoom);
}

void Camera2D::setCenter(const sf::Vector2f& worldCenter) {
  m_view.setCenter(worldCenter);
}

sf::Vector2f Camera2D::screenToWorld(const sf::Vector2f& screenInternal) const {
  const sf::Vector2f viewSize = m_view.getSize();
  const sf::Vector2f topLeft =
    m_view.getCenter() - viewSize * 0.5f;

  const float sx = viewSize.x / m_baseSize.x;
  const float sy = viewSize.y / m_baseSize.y;

  return topLeft + sf::Vector2f(
    screenInternal.x * sx,
    screenInternal.y * sy
  );
}

sf::Vector2f Camera2D::windowToWorld(const sf::RenderWindow& window,
                                     const sf::Vector2i& pixel) const {
  return window.mapPixelToCoords(pixel, m_view);
}
