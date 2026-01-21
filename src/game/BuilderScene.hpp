#pragma once

#include "engine/Scene.hpp"
#include "engine/Input.hpp"
#include "graphics/Camera2D.hpp"

#include "world/City.hpp"
#include "world/TilemapRenderer.hpp"

#include "game/BuildTool.hpp"
#include "game/SidePanel.hpp"

#include <SFML/Graphics.hpp>
#include "ui/Button.hpp"
#include <memory>

class BuilderScene : public Scene
{
public:
  BuilderScene(sf::RenderWindow &window,
               float internalW,
               float internalH,
               int tileSize);

  void beginFrame() override;
  void handleEvent(const sf::Event &e) override;
  void update(float dt) override;
  void render(sf::RenderTarget &target) override;

private:
  // --- Core ---
  sf::RenderWindow &m_window;
  Button m_panelButton;

  Input m_input;
  Camera2D m_camera;
  BuildTool m_activeTool = BuildTool::None;

  // --- World ---
  City m_city;
  TilemapRenderer m_renderer;

  int m_hoverTx = 0;
  int m_hoverTy = 0;

  std::unique_ptr<Placeable> m_ghost;
  bool m_canPlaceGhost = false;

  // --- UI ---
  sf::View m_uiView;

  SidePanel m_panel;

  // --- Helpers ---
  sf::Vector2f windowMouseToInternal() const;
  void updateHoverTile();
};
