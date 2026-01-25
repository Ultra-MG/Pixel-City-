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
#include "world/MoneyProducer.hpp"
#include "game/Inventory.hpp"
#include "game/InventoryPanel.hpp"

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
#include <unordered_map>

struct CropType
{
  std::string name;
  std::function<Crop *()> create;
  std::string iconPath;
};

enum class DeleteMode
{
  Delete,
  Sell
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
  Button m_inventoryButton;

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

  // --- Upgrade Prompt ---
  bool m_upgradePromptVisible = false;
  Placeable *m_upgradeTarget = nullptr;
  sf::RectangleShape m_upgradeBg;
  sf::Text m_upgradeText;
  PanelButton m_upgradeYes;
  PanelButton m_upgradeNo;
  void showUpgradePrompt(const sf::Vector2f &uiPos, Placeable *p);

  // --- UI ---
  sf::View m_uiView;
  sf::Text m_moneyText;
  sf::Text m_diamondText;
  sf::Texture m_coinTexture;
  sf::Texture m_diamondTexture;
  sf::Sprite m_coinSprite;
  sf::Sprite m_diamondSprite;
  sf::Text m_toastText;
  float m_toastTimer = 0.f;

  struct PopupText
  {
    sf::Text text;
    sf::Vector2f pos;
    float timer = 0.f;
  };
  std::vector<PopupText> m_popups;
  std::unordered_map<std::string, sf::Texture> m_collectIcons;

  bool m_sellPromptVisible = false;
  std::string m_sellItemId;
  int m_sellQty = 1;
  int m_sellPrice = 0;
  sf::RectangleShape m_sellBg;
  sf::Text m_sellTitle;
  sf::Text m_sellQtyText;
  sf::Text m_sellPriceText;
  PanelButton m_sellPlus;
  PanelButton m_sellMinus;
  PanelButton m_sellConfirm;
  PanelButton m_sellCancel;

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
  InventoryPanel m_inventoryPanel;

  Wallet m_wallet{1000, 20};
  Inventory m_inventory;
  std::string m_cityName;
  bool m_saveEnabled = true;
  std::int64_t m_timeAccMs = 0;
  EconomySystem m_economy;
  DeleteMode m_deleteMode = DeleteMode::Delete;

  // --- Helpers ---
  sf::Vector2f windowMouseToInternal() const;
  void updateHoverTile();
  void positionCropMenu(const sf::Vector2f &uiPos);
  void updateCurrencyUI();
  void saveGame();
  void showToast(const std::string &text, float seconds);
  void showDeletePrompt(const sf::Vector2f &uiPos, Placeable *target);
  void addPopup(const sf::Vector2f &worldPos, const std::string &text, float seconds);
  void loadFont();
  void loadTextures();
  void initDeleteButtons();
  void initCrops();
  void initCurrencyUi();
  void initPanelButton();
  void initInventoryPanel();
  void showSellPrompt(const sf::Vector2f &uiPos, const std::string &itemId);
  void updateSellPrompt();
  void restoreState(const std::optional<GameState> &state);
};
