#include "world/TilemapRenderer.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <stdexcept>

TilemapRenderer::TilemapRenderer(int tileSize,
                                 const char* grassPath,
                                 const char* roadPath,
                                 const char* housePath)
: m_tile(tileSize)
{
  if (!m_grassTex.loadFromFile(grassPath))
    throw std::runtime_error("Failed to load grass PNG");
  m_grassTex.setSmooth(false);
  m_grassSprite.setTexture(m_grassTex);
  scaleSpriteToTile(m_grassSprite, m_grassTex);

  if (!m_roadTex.loadFromFile(roadPath))
    throw std::runtime_error("Failed to load road PNG");
  m_roadTex.setSmooth(false);
  m_roadSprite.setTexture(m_roadTex);
  scaleSpriteToTile(m_roadSprite, m_roadTex);

  if (!m_houseTex.loadFromFile(housePath))
    throw std::runtime_error("Failed to load house PNG");
  m_houseTex.setSmooth(false);
  m_houseSprite.setTexture(m_houseTex);
  m_houseSprite.setColor(sf::Color::White);
}

void TilemapRenderer::scaleSpriteToTile(sf::Sprite& spr, const sf::Texture& tex) const {
  const auto s = tex.getSize();
  if (s.x == 0 || s.y == 0) return;
  spr.setScale((float)m_tile / (float)s.x, (float)m_tile / (float)s.y);
}

void TilemapRenderer::draw(sf::RenderTarget& target, const City& city) const {
  // Draw tiles (grass / road)
  for (int y = 0; y < city.h(); ++y) {
    for (int x = 0; x < city.w(); ++x) {
      const float px = (float)(x * m_tile);
      const float py = (float)(y * m_tile);

      if (city.getTile(x, y) == Tile::Road) {
        m_roadSprite.setPosition(px, py);
        target.draw(m_roadSprite);
      } else {
        m_grassSprite.setPosition(px, py);
        target.draw(m_grassSprite);
      }
    }
  }

  // Draw buildings scaled to their footprint
  for (const auto& build : city.buildings()) {
    switch (build.type) {
      case BuildingType::House: {
        const auto hs = m_houseTex.getSize();
        if (hs.x == 0 || hs.y == 0) break;

        const float px = (float)(build.x * m_tile);
        const float py = (float)(build.y * m_tile);
        const float pw = (float)(build.w * m_tile);
        const float ph = (float)(build.h * m_tile);

        m_houseSprite.setPosition(px, py);
        m_houseSprite.setScale(pw / (float)hs.x, ph / (float)hs.y);

        // Optional: tint slightly by level so upgrades are visible
        const int boost = std::min(60, (build.level - 1) * 10);
        m_houseSprite.setColor(sf::Color(255, 255 - boost, 255 - boost, 255));

        target.draw(m_houseSprite);
        m_houseSprite.setColor(sf::Color::White);
      } break;

      default:
        break;
    }
  }
}

void TilemapRenderer::drawTileHover(sf::RenderTarget& target, int tx, int ty) const {
  sf::RectangleShape h;
  h.setPosition((float)(tx * m_tile), (float)(ty * m_tile));
  h.setSize({(float)m_tile, (float)m_tile});
  h.setFillColor(sf::Color(255, 255, 255, 35));
  target.draw(h);
}

void TilemapRenderer::drawGhostHouse(sf::RenderTarget& target, int tx, int ty, bool valid) const {
  const auto hs = m_houseTex.getSize();
  if (hs.x == 0 || hs.y == 0) return;

  const float px = (float)(tx * m_tile);
  const float py = (float)(ty * m_tile);

  // Ghost footprint is 2x2 tiles (matches placement)
  const float pw = (float)(2 * m_tile);
  const float ph = (float)(2 * m_tile);

  m_houseSprite.setPosition(px, py);
  m_houseSprite.setScale(pw / (float)hs.x, ph / (float)hs.y);
  m_houseSprite.setColor(valid ? sf::Color(80, 255, 120, 160)
                               : sf::Color(255, 80, 80, 160));

  target.draw(m_houseSprite);
  m_houseSprite.setColor(sf::Color::White);
}

// NEW: Road ghost preview (1 tile)
void TilemapRenderer::drawGhostRoad(sf::RenderTarget& target, int tx, int ty, bool valid) const {
  const float px = (float)(tx * m_tile);
  const float py = (float)(ty * m_tile);

  m_roadSprite.setPosition(px, py);
  m_roadSprite.setColor(valid ? sf::Color(80, 255, 120, 160)
                              : sf::Color(255, 80, 80, 160));

  target.draw(m_roadSprite);
  m_roadSprite.setColor(sf::Color::White);
}
