#pragma once
#include "engine/Scene.hpp"
#include "engine/Input.hpp"
#include "graphics/Camera2D.hpp"
#include "world/City.hpp"
#include "world/TilemapRenderer.hpp"
#include "game/BuildTool.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

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

  // Start in "grass only" mode (no building / no road)
  BuildTool m_tool = BuildTool::None;

  // Hover tile coords
  int m_hoverTx = 0;
  int m_hoverTy = 0;

  // UI
  bool m_constructOpen = false;

  // UI is drawn in INTERNAL pixel coordinates
  sf::View m_uiView;

  // Optional font for labels
  bool m_fontOk = false;
  sf::Font m_font;

  // Helpers
  sf::Vector2f windowMouseToInternal() const;
  void updateHoverTile();

  sf::FloatRect rectConstructButton() const;
  sf::FloatRect rectHouseButton() const;
  sf::FloatRect rectRoadButton() const;

  bool mouseIn(const sf::FloatRect& r, const sf::Vector2f& mouseInternal) const;

  void drawButton(sf::RenderTarget& target,
                  const sf::FloatRect& r,
                  bool active,
                  const char* label,
                  sf::Color iconColor) const;
};