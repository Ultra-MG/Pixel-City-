#include "graphics/PixelRenderer.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

PixelRenderer::PixelRenderer(unsigned internalW, unsigned internalH, unsigned scale)
: m_scale(scale) {
  m_lowRes.create(internalW, internalH);
  m_lowRes.setSmooth(false);

  m_present.setTexture(m_lowRes.getTexture());
  m_present.setScale(static_cast<float>(m_scale), static_cast<float>(m_scale));
}

void PixelRenderer::begin() {
  m_lowRes.clear(sf::Color(20, 20, 24));
}

sf::RenderTarget& PixelRenderer::target() {
  return m_lowRes;
}

void PixelRenderer::endAndPresent(sf::RenderTarget& window) {
  m_lowRes.display();
  window.draw(m_present);
}
