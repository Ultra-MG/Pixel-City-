#pragma once

#include "engine/Scene.hpp"
#include "engine/Input.hpp"
#include "graphics/Camera2D.hpp"

#include "world/City.hpp"
#include "world/TilemapRenderer.hpp"

#include "game/BuildTool.hpp"
#include "game/SidePanel.hpp"
#include "game/Economy.hpp"
#include "game/SaveSystem.hpp"

#include <SFML/Graphics.hpp>
#include "ui/Button.hpp"
#include "world/Crop.hpp"
#include "game/EconomySystem.hpp"
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "ui/PanelButton.hpp"

struct CropType
{
  std::string name;
  std::function<Crop *()> create;
  std::string iconPath;
};

class BuilderScene : public Scene
{
public:
  BuilderScene(sf::RenderWindow &window,
               float internalW,
               float internalH,
               int tileSize,
               const std::string &cityName,
               const std::optional<GameState> &state = std::nullopt);

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
  Placeable *m_selectedCropField = nullptr;
  bool m_cropMenuVisible = false;

  // --- UI ---
  sf::View m_uiView;
  sf::Text m_moneyText;
  sf::Text m_diamondText;
  std::optional<sf::Texture> m_coinTexture;
  std::optional<sf::Texture> m_diamondTexture;
  std::optional<sf::Sprite> m_coinSprite;
  std::optional<sf::Sprite> m_diamondSprite;
  std::optional<sf::Text> m_toastText;
  float m_toastTimer = 0.f;

  bool m_deletePromptVisible = false;
  Placeable *m_deleteTarget = nullptr;
  sf::RectangleShape m_deleteBg;
  std::optional<sf::Text> m_deleteLabel;
  PanelButton m_deleteYes;
  PanelButton m_deleteNo;
  float m_holdTime = 0.f;
  Placeable *m_holdTarget = nullptr;
  const float m_holdThreshold = 0.6f;

  SidePanel m_panel;

  Wallet m_wallet{1000, 20};
  std::string m_cityName;
  bool m_saveEnabled = true;
  std::int64_t m_timeAccMs = 0;
  EconomySystem m_economy;

  // --- Helpers ---
  sf::Vector2f windowMouseToInternal() const;
  void updateHoverTile();
  void positionCropMenu(const sf::Vector2f &uiPos);
  void updateCurrencyUI();
  void saveGame();
  void showToast(const std::string &text, float seconds);
  void showDeletePrompt(const sf::Vector2f &uiPos, Placeable *target);
};
