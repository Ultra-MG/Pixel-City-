#include "graphics/PixelRenderer.hpp"
#include <SFML/Graphics/Color.hpp>

PixelRenderer::PixelRenderer(unsigned internalW,
                             unsigned internalH,
                             unsigned scale)
: m_scale(scale)
, m_lowRes(sf::Vector2u(internalW, internalH))
, m_present(m_lowRes.getTexture())
{
  m_lowRes.setSmooth(false);

  m_present.setScale(
    sf::Vector2f(
      static_cast<float>(m_scale),
      static_cast<float>(m_scale)
    )
  );
}

void PixelRenderer::begin()
{
    m_lowRes.setView(m_lowRes.getDefaultView()); 
    m_lowRes.clear(sf::Color(20, 20, 24));
}


sf::RenderTarget& PixelRenderer::target() {
  return m_lowRes;
}

void PixelRenderer::endAndPresent(sf::RenderTarget& window) {
  m_lowRes.display();

  window.setView(window.getDefaultView()); // ← THIS WAS MISSING
  window.draw(m_present);
}

