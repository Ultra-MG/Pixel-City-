#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include "world/City.hpp"

class TilemapRenderer {
public:
  TilemapRenderer(int tileSize, const char* grassPath);

  void drawTiles(sf::RenderTarget& target, const City& city) const;
  void drawTileHover(sf::RenderTarget& target, int tx, int ty) const;

private:
  int m_tile = 0;

  sf::Texture m_grassTex;
  mutable std::optional<sf::Sprite> m_grassSprite;

  void scaleSpriteToTile(sf::Sprite& spr,
                         const sf::Texture& tex) const;
};
