#pragma once

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class PixelRenderer {
public:
  PixelRenderer(unsigned internalW, unsigned internalH, unsigned scale);

  void begin();
  sf::RenderTarget& target();
  void endAndPresent(sf::RenderTarget& window);

private:
  sf::RenderTexture m_lowRes;
  sf::Sprite m_present;
  unsigned m_scale = 1;
};
