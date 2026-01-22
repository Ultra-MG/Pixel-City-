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
#include "world/Crop.hpp"
#include <functional>
#include <memory>
#include <string>
#include <vector>

struct CropType {
    std::string name;
    std::function<Crop*()> create;
    std::string iconPath;
};

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

  // --- Crop Menu ---
  std::vector<CropType> m_cropTypes;
  std::vector<Button> m_cropButtons;
  std::vector<sf::Text> m_cropLabels;
  sf::RectangleShape m_cropMenuBg;
  sf::Font m_uiFont;
  Placeable* m_selectedCropField = nullptr;
  bool m_cropMenuVisible = false;

  // --- UI ---
  sf::View m_uiView;

  SidePanel m_panel;

  // --- Helpers ---
  sf::Vector2f windowMouseToInternal() const;
  void updateHoverTile();
  void positionCropMenu(const sf::Vector2f &uiPos);
};
