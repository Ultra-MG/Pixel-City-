#include "world/TilemapRenderer.hpp"
#include <stdexcept>

TilemapRenderer::TilemapRenderer(int tileSize, const char* grassPath)
: m_tile(tileSize)
{
  if (!m_grassTex.loadFromFile(grassPath))
    throw std::runtime_error("Failed to load grass PNG");

  m_grassSprite.emplace(m_grassTex);
  scaleSpriteToTile(*m_grassSprite, m_grassTex);
}

void TilemapRenderer::scaleSpriteToTile(sf::Sprite& spr,
                                        const sf::Texture& tex) const
{
  const auto s = tex.getSize();
  spr.setScale({
    float(m_tile) / s.x,
    float(m_tile) / s.y
  });
}

void TilemapRenderer::drawTiles(sf::RenderTarget& target,
                                const City& city) const
{
  for (int y = 0; y < city.h(); ++y) {
    for (int x = 0; x < city.w(); ++x) {
      m_grassSprite->setPosition({
        float(x * m_tile),
        float(y * m_tile)
      });
      target.draw(*m_grassSprite);
    }
  }
}
