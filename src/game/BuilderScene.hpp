#pragma once
#include "engine/Scene.hpp"
#include "engine/Input.hpp"
#include "graphics/Camera2D.hpp"
#include "world/City.hpp"
#include "world/TilemapRenderer.hpp"
#include "game/BuildTool.hpp"
#include "game/RadialMenu.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Font.hpp>

class BuilderScene : public Scene {
public:
  BuilderScene(sf::RenderWindow& window, float internalW, float internalH, int tileSize);

  void beginFrame() override;
  void handleEvent(const sf::Event& e) override;
  void update(float dt) override;
  void render(sf::RenderTarget& target) override;

private:
  sf::RenderWindow& m_window;

  Input m_input;
  Camera2D m_camera;

  City m_city;
  TilemapRenderer m_renderer;

  BuildTool m_tool = BuildTool::None;

  int m_hoverTx = 0;
  int m_hoverTy = 0;

  // UI
  bool m_constructOpen = false;
  sf::View m_uiView;

  // Optional font (not needed for wheel if you use icons only)
  bool m_fontOk = false;
  sf::Font m_font;

  // NEW
  RadialMenu m_wheel;

  // Ghost validity cache
  bool m_canPlaceGhostHouse = false;
  bool m_canPlaceGhostRoad  = false;

  sf::Vector2f windowMouseToInternal() const;
  void updateHoverTile();
};
