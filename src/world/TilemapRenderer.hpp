#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "world/City.hpp"

class TilemapRenderer {
public:
  TilemapRenderer(int tileSize,
                  const char* grassPath,
                  const char* roadPath,
                  const char* housePath);

  void draw(sf::RenderTarget& target, const City& city) const;
  void drawGhostHouse(sf::RenderTarget& target, int tx, int ty, bool valid) const;
  void drawTileHover(sf::RenderTarget& target, int tx, int ty) const;

private:
  int m_tile;

  sf::Texture m_grassTex;
  mutable sf::Sprite m_grassSprite;

  sf::Texture m_roadTex;
  mutable sf::Sprite m_roadSprite;

  sf::Texture m_houseTex;
  mutable sf::Sprite m_houseSprite;

  void scaleSpriteToTile(sf::Sprite& spr, const sf::Texture& tex) const;
};
