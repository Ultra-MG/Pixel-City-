#include "game/BuilderScene.hpp"
#include "core/Config.hpp"
#include "world/Buildings/House.hpp"
#include "world/Buildings/Farm.hpp"
#include "world/Buildings/Store.hpp"
#include "world/Buildings/TownHall.hpp"
#include "world/Buildings/Bakery.hpp"
#include "world/Infrastructure/Road.hpp"
#include "world/Infrastructure/Water.hpp"
#include "world/Infrastructure/Bridge.hpp"
#include "world/Infrastructure/CropField.hpp"
#include "world/Crops/Wheat.hpp"
#include "world/Crops/Corn.hpp"
#include "world/Crops/Carrot.hpp"
#include "world/Crops/Potato.hpp"
#include "world/CropRegistry.hpp"
#include <cmath>
#include "game/screens/MainMenuScreen.hpp"
#include "world/Decorations/Fountain.hpp"
#include "world/Decorations/Tree.hpp"
#include "world/Decorations/Bench.hpp"
#include "world/Decorations/LampPost.hpp"
#include "game/BuildToolFactory.hpp"
#include "game/Wallet.hpp"
#include "world/Building.hpp"
#include <SFML/Window/Mouse.hpp>
#include <stdexcept>
#include <algorithm>

BuilderScene::BuilderScene(sf::RenderWindow &window,
                           SceneManager &scenes,
                           float internalW,
                           float internalH,
                           int tileSize,
                           const std::string &cityName,
                           const std::optional<GameState> &state)
    : m_window(window), m_upgradeText(m_uiFont, "", 8), m_sellTitle(m_uiFont, "", 10), m_sellQtyText(m_uiFont, "", 10), m_sellPriceText(m_uiFont, "", 10), m_camera(internalW, internalH), m_city(cfg::CityW, cfg::CityH), m_renderer(tileSize, "assets/grass.png"), m_panel(40.f, 160.f, internalH), m_inventoryPanel(40.f, 160.f, internalH), m_moneyText(m_uiFont, "0", 10), m_diamondText(m_uiFont, "0", 10), m_coinSprite(m_coinTexture), m_diamondSprite(m_diamondTexture), m_toastText(m_uiFont, "", 10), m_economy(m_wallet), m_scenes(scenes)
{

  restoreState(state);
  loadFont();
  loadTextures();

  m_camera.setZoomClamp(cfg::ZoomMin, cfg::ZoomMax);

  const float mapW = float(cfg::CityW * cfg::TileSize);
  const float mapH = float(cfg::CityH * cfg::TileSize);
  m_camera.setCenter({mapW * 0.5f, mapH * 0.5f});

  m_uiView.setSize({internalW, internalH});
  m_uiView.setCenter({internalW * 0.5f, internalH * 0.5f});

  m_cityName = cityName;

  initDeleteButtons();
  initCrops();
  initCurrencyUi();
  initPanelButton();
  createExitButton();
  initInventoryPanel();
  updateCurrencyUI();

  saveGame();
}

void BuilderScene::createExitButton()
{
  m_exitBtn.setSize({20.f, 20.f});
  m_exitBtn.setFillColor(sf::Color(30, 30, 30, 220));
  m_exitBtn.setOutlineColor(sf::Color(120, 80, 85));
  m_exitBtn.setOutlineThickness(1.f);
  m_exitBtn.setPosition({495.f, 315.f});

  m_exitText.emplace(m_uiFont, "X", 10);
  m_exitText->setFillColor(sf::Color(245, 240, 220));

  const sf::FloatRect eb = m_exitText->getLocalBounds();
  m_exitText->setOrigin({eb.position.x + eb.size.x * 0.5f,
                         eb.position.y + eb.size.y * 0.5f});
  m_exitText->setPosition({m_exitBtn.getPosition().x + 10.f,
                           m_exitBtn.getPosition().y + 10.f});
}

void BuilderScene::initCurrencyUi()
{
  if (!m_coinTexture.loadFromFile("assets/coin.png"))
    throw std::runtime_error("BuilderScene: failed to load assets/coin.png");

  if (!m_diamondTexture.loadFromFile("assets/diamond.png"))
    throw std::runtime_error("BuilderScene: failed to load assets/diamond.png");

  m_coinTexture.setSmooth(false);
  m_diamondTexture.setSmooth(false);

  m_coinSprite.setTexture(m_coinTexture, true);
  m_diamondSprite.setTexture(m_diamondTexture, true);

  m_moneyText.setFillColor(sf::Color::White);
  m_diamondText.setFillColor(sf::Color::White);

  m_toastText.setFillColor(sf::Color(255, 230, 180));
}

void BuilderScene::initPanelButton()
{
  m_panelButton = Button({20.f, 20.f}, {6.f, 6.f});
  m_panelButton.setOutline(sf::Color(70, 70, 75), 1.f);

  m_panelButton.loadImage("assets/ui/hammer.png");
  m_panelButton.setImageSize({20.f, 20.f});
  m_panelButton.setImageOffset({0.f, 0.f});
}

void BuilderScene::initInventoryPanel()
{
  const float y = m_uiView.getSize().y - 26.f;
  m_inventoryButton = Button({20.f, 20.f}, {6.f, y});
  m_inventoryButton.setOutline(sf::Color(70, 70, 75), 1.f);
  m_inventoryButton.loadImage("assets/ui/inventory.png");
  m_inventoryButton.setImageSize({20.f, 20.f});
  m_inventoryButton.setImageOffset({0.f, 0.f});

  m_sellBg.setSize({180.f, 70.f});
  m_sellBg.setFillColor(sf::Color(30, 30, 30, 220));
  m_sellBg.setOutlineColor(sf::Color(120, 80, 85));
  m_sellBg.setOutlineThickness(1.f);
  m_sellTitle.setFillColor(sf::Color(245, 240, 220));
  m_sellQtyText.setFillColor(sf::Color(245, 240, 220));
  m_sellPriceText.setFillColor(sf::Color(255, 220, 120));
  m_sellPlus = PanelButton(sf::Vector2f{18.f, 18.f}, sf::Vector2f{0.f, 0.f});
  m_sellMinus = PanelButton(sf::Vector2f{18.f, 18.f}, sf::Vector2f{0.f, 0.f});
  m_sellConfirm = PanelButton(sf::Vector2f{52.f, 18.f}, sf::Vector2f{0.f, 0.f});
  m_sellCancel = PanelButton(sf::Vector2f{52.f, 18.f}, sf::Vector2f{0.f, 0.f});
  m_sellPlus.setOutline(sf::Color(120, 80, 85), 1.f);
  m_sellMinus.setOutline(sf::Color(120, 80, 85), 1.f);
  m_sellConfirm.setOutline(sf::Color(120, 80, 85), 1.f);
  m_sellCancel.setOutline(sf::Color(120, 80, 85), 1.f);
  m_sellPlus.setText(m_uiFont, "+", 10, sf::Color::Black);
  m_sellMinus.setText(m_uiFont, "-", 10, sf::Color::Black);
  m_sellConfirm.setText(m_uiFont, "Confirm", 7, sf::Color::Black);
  m_sellCancel.setText(m_uiFont, "Cancel", 7, sf::Color::Black);
}

void BuilderScene::restoreState(const std::optional<GameState> &state)
{
  if (state)
  {
    m_saveEnabled = false;
    SaveSystem::applyState(*state, m_city, m_wallet, m_inventory);
    if (!state->cityName.empty())
      m_cityName = state->cityName;

    const std::int64_t now = std::time(nullptr);
    const std::int64_t delta = now - state->lastSaveTimestamp;

    if (delta > 0)
    {
      for (auto &o : m_city.objects())
      {
        if (auto *p = dynamic_cast<MoneyProducer *>(o.get()))
          p->applyOffline(delta,m_inventory);
      }
    }
    m_saveEnabled = true;
  }
  else
  {
    m_city.place(std::make_unique<TownHall>(
        cfg::CityW / 2 - 2,
        cfg::CityH / 2 - 2));
  }
}

void BuilderScene::initDeleteButtons()
{
  m_deleteBg.setSize({140.f, 48.f});
  m_deleteBg.setFillColor(sf::Color(30, 30, 30, 220));
  m_deleteBg.setOutlineColor(sf::Color(120, 80, 85));
  m_deleteBg.setOutlineThickness(1.f);
  m_deleteLabel.emplace(m_uiFont, "Delete object?", 10);
  m_deleteLabel->setFillColor(sf::Color(245, 240, 220));
  m_deleteYes = PanelButton(sf::Vector2f{50.f, 18.f}, sf::Vector2f{0.f, 0.f});
  m_deleteNo = PanelButton(sf::Vector2f{50.f, 18.f}, sf::Vector2f{0.f, 0.f});
  m_deleteYes.setOutline(sf::Color(120, 80, 85), 1.f);
  m_deleteNo.setOutline(sf::Color(120, 80, 85), 1.f);
  m_deleteYes.setText(m_uiFont, "Yes", 7, sf::Color::Black);
  m_deleteNo.setText(m_uiFont, "No", 7, sf::Color::Black);
}
void BuilderScene::initCrops()
{

  m_cropTypes.emplace_back(CropType{"Wheat", []()
                                    { return new Wheat(); }, "assets/crops/wheat.png"});
  m_cropTypes.emplace_back(CropType{"Corn", []()
                                    { return new Corn(); }, "assets/crops/corn.png"});
  m_cropTypes.emplace_back(CropType{"Carrot", []()
                                    { return new Carrot(); }, "assets/crops/carrot.png"});
  m_cropTypes.emplace_back(CropType{"Potato", []()
                                    { return new Potato(); }, "assets/crops/potato.png"});

  const sf::Vector2f buttonSize{40.f, 40.f};
  for (size_t i = 0; i < m_cropTypes.size(); ++i)
  {
    m_cropButtons.emplace_back(Button(buttonSize, {0.f, 0.f}));
    m_cropButtons.back().loadImage(m_cropTypes[i].iconPath);
    m_cropButtons.back().setImageSize({32.f, 32.f});
    m_cropButtons.back().setImageOffset({4.f, 4.f});
    m_cropLabels.emplace_back(m_uiFont, m_cropTypes[i].name, 10);
    m_cropLabels.back().setFillColor(sf::Color::White);
  }

  m_cropMenuBg.setSize({170.f, 140.f});
  m_cropMenuBg.setFillColor(sf::Color(30, 30, 30, 220));
  m_cropMenuBg.setOutlineColor(sf::Color(120, 80, 85));
  m_cropMenuBg.setOutlineThickness(1.f);
  m_cropMenuBg.setPosition({0.f, 0.f});
}
void BuilderScene::loadTextures()
{
  Road::loadTexture();
  Water::loadTexture();
  Bridge::loadTexture();
  CropField::loadTexture();
  House::loadTexture();
  TownHall::loadTexture();
  Farm::loadTexture();
  Store::loadTexture();
  Tree::loadTexture();
  Fountain::loadTexture();
  Bench::loadTexture();
  LampPost::loadTexture();
  Wheat::loadTexture();
  Corn::loadTexture();
  Carrot::loadTexture();
  Potato::loadTexture();
  Bakery::loadTexture();

  m_collectIcons.clear();
  sf::Texture coin;
  if (coin.loadFromFile("assets/coin.png"))
  {
    coin.setSmooth(false);
    m_collectIcons.emplace("coin", std::move(coin));
  }
  for (const auto &crop : CropRegistry::all())
  {
    sf::Texture tex;
    if (tex.loadFromFile(crop.iconPath))
    {
      tex.setSmooth(false);
      m_collectIcons.emplace(crop.id, std::move(tex));
    }
  }
}
void BuilderScene::beginFrame()
{
  m_input.beginFrame();
}

void BuilderScene::handleEvent(const sf::Event &e)
{
  m_input.handleEvent(e);
}

sf::Vector2f BuilderScene::windowMouseToInternal() const
{
  const sf::Vector2i mp = sf::Mouse::getPosition(m_window);
  return {
      float(mp.x) / float(cfg::Scale),
      float(mp.y) / float(cfg::Scale)};
}

void BuilderScene::updateHoverTile()
{
  const sf::Vector2i mp = sf::Mouse::getPosition(m_window);
  const sf::Vector2f world = m_camera.windowToWorld(m_window, mp);

  m_hoverTx = std::clamp(int(world.x) / cfg::TileSize, 0, m_city.w() - 1);
  m_hoverTy = std::clamp(int(world.y) / cfg::TileSize, 0, m_city.h() - 1);
}

void BuilderScene::positionCropMenu(const sf::Vector2f &uiPos)
{
  const sf::Vector2f menuSize = m_cropMenuBg.getSize();
  const sf::Vector2f viewSize = m_uiView.getSize();
  const sf::Vector2f padding{8.f, 8.f};

  sf::Vector2f pos = uiPos + sf::Vector2f{12.f, 12.f};
  pos.x = std::clamp(pos.x, padding.x, viewSize.x - menuSize.x - padding.x);
  pos.y = std::clamp(pos.y, padding.y, viewSize.y - menuSize.y - padding.y);
  m_cropMenuBg.setPosition(pos);

  const sf::Vector2f buttonSize{40.f, 40.f};
  const float buttonStartX = pos.x + 12.f;
  const float buttonStartY = pos.y + 12.f;
  const float colSpacing = 75.f;
  const float rowSpacing = 60.f;
  const int cols = 2;

  for (size_t i = 0; i < m_cropButtons.size(); ++i)
  {
    const int col = static_cast<int>(i) % cols;
    const int row = static_cast<int>(i) / cols;
    const float bx = buttonStartX + col * colSpacing;
    const float by = buttonStartY + row * rowSpacing;

    m_cropButtons[i] = Button(buttonSize, {bx, by});
    m_cropButtons[i].loadImage(m_cropTypes[i].iconPath);
    m_cropButtons[i].setImageSize({32.f, 32.f});
    m_cropButtons[i].setImageOffset({4.f, 4.f});

    m_cropLabels[i].setPosition({bx - 2.f, by + 44.f});
  }
}

void BuilderScene::updateCurrencyUI()
{
  const sf::Vector2f viewSize = m_uiView.getSize();
  const sf::Vector2f padding{6.f, 6.f};

  m_moneyText.setString(std::to_string(m_wallet.money()));
  m_diamondText.setString(std::to_string(m_wallet.diamonds()));

  const float iconSize = 12.f;
  const float gap = 4.f;
  const float groupGap = 10.f;

  const sf::FloatRect moneyBounds = m_moneyText.getLocalBounds();
  const sf::FloatRect diamondBounds = m_diamondText.getLocalBounds();
  const float moneyWidth = moneyBounds.size.x;
  const float diamondWidth = diamondBounds.size.x;

  const float totalWidth =
      iconSize + gap + moneyWidth +
      groupGap +
      iconSize + gap + diamondWidth;

  const float startX = viewSize.x - padding.x - totalWidth;
  const float y = padding.y;

  const sf::Vector2u coinSize = m_coinTexture.getSize();
  if (coinSize.x > 0 && coinSize.y > 0)
  {
    const float scale = iconSize / static_cast<float>(std::max(coinSize.x, coinSize.y));
    m_coinSprite.setScale({scale, scale});
  }
  const sf::Vector2u diamondSize = m_diamondTexture.getSize();
  if (diamondSize.x > 0 && diamondSize.y > 0)
  {
    const float scale = iconSize / static_cast<float>(std::max(diamondSize.x, diamondSize.y));
    m_diamondSprite.setScale({scale, scale});
  }

  m_coinSprite.setPosition({startX, y + 2.f});
  m_moneyText.setPosition({startX + iconSize + gap, y});
  const float diamondStart = startX + iconSize + gap + moneyWidth + groupGap;
  m_diamondSprite.setPosition({diamondStart, y + 2.f});
  m_diamondText.setPosition({diamondStart + iconSize + gap, y});
}

void BuilderScene::update(float dt)
{

  for (auto &o : m_city.objects())
    o->update(dt);
  const sf::Vector2i mp = sf::Mouse::getPosition(m_window);
  const sf::Vector2f mouseUI =
      m_window.mapPixelToCoords(mp, m_uiView);
  const sf::Vector2f mouseWorld = m_camera.windowToWorld(m_window, mp);

  const bool leftClick = m_input.leftPressed();
  const bool rightClick = m_input.rightPressed();
  const bool leftDown = m_input.leftDown();

  if (m_upgradePromptVisible)
  {
    if (leftClick)
    {
      if (m_upgradeYes.contains(mouseUI))
      {
        const Cost cost = m_upgradeTarget->upgradeCost();
        if (m_wallet.spend(cost))
        {
          m_upgradeTarget->upgrade(m_city);
          updateCurrencyUI();
          saveGame();
          showToast("Upgraded!", 1.2f);
        }
        else
        {
          showToast("Not enough money", 1.2f);
        }

        m_upgradePromptVisible = false;
        m_upgradeTarget = nullptr;
        return;
      }

      if (m_upgradeNo.contains(mouseUI))
      {
        m_upgradePromptVisible = false;
        m_upgradeTarget = nullptr;
        return;
      }
    }
  }

  const float wheel = m_input.wheelDelta();
  if (wheel != 0.f)
  {
    const float zoomFactor = (wheel > 0.f) ? 1.1f : 0.9f;
    m_camera.zoom(zoomFactor);
  }

  if (m_input.draggingRight())
  {
    const sf::Vector2i drag = m_input.dragDelta();
    const sf::Vector2f originWorld = m_camera.screenToWorld({0.f, 0.f});
    const sf::Vector2f dragWorld =
        m_camera.screenToWorld({static_cast<float>(drag.x), static_cast<float>(drag.y)});
    const sf::Vector2f deltaWorld = dragWorld - originWorld;
    m_camera.move(-deltaWorld);
  }

  updateHoverTile();

  if (m_toastTimer > 0.f)
  {
    m_toastTimer -= dt;
    if (m_toastTimer <= 0.f)
      m_toastTimer = 0.f;
  }

  if (!m_popups.empty())
  {
    for (auto &p : m_popups)
    {
      p.timer -= dt;
      p.pos.y -= dt * 6.f;
    }
    m_popups.erase(
        std::remove_if(m_popups.begin(), m_popups.end(),
                       [](const PopupText &p)
                       { return p.timer <= 0.f; }),
        m_popups.end());
  }

  m_timeAccMs += static_cast<std::int64_t>(dt * 1000);

  if (m_timeAccMs >= 1000)
  {
    const std::int64_t seconds = m_timeAccMs / 1000;
    m_timeAccMs %= 1000;

    for (auto &o : m_city.objects())
    {
      if (auto *p = dynamic_cast<MoneyProducer *>(o.get()))
        p->tick(seconds,m_inventory);
    }
  }

  if (m_panel.isOpen())
  {
    m_panelButton.loadImage("assets/ui/close.png");
    m_panelButton.setImageSize({18.f, 18.f});
    m_panelButton.setImageOffset({1.f, 1.f});
  }
  else
  {
    m_panelButton.loadImage("assets/ui/hammer.png");
    m_panelButton.setImageSize({20.f, 20.f});
    m_panelButton.setImageOffset({0.f, 0.f});
  }

  if (m_inventoryPanel.isOpen())
  {
    m_inventoryButton.loadImage("assets/ui/close.png");
    m_inventoryButton.setImageSize({18.f, 18.f});
    m_inventoryButton.setImageOffset({1.f, 1.f});
  }
  else
  {
    m_inventoryButton.loadImage("assets/ui/inventory.png");
    m_inventoryButton.setImageSize({20.f, 20.f});
    m_inventoryButton.setImageOffset({0.f, 0.f});
  }

  if (m_deletePromptVisible)
  {
    if (leftClick)
    {
      if (m_deleteYes.contains(mouseUI))
      {
        if (m_deleteTarget)
        {
          if (m_deleteMode == DeleteMode::Sell)
          {
            const Cost refund = m_deleteTarget->sellValue();
            m_wallet.add(refund);
            updateCurrencyUI();
          }

          if (m_city.removePlaceable(m_deleteTarget))
            saveGame();
        }

        m_deletePromptVisible = false;
        m_deleteTarget = nullptr;
        return;
      }
      return;
    }
    if (m_deleteNo.contains(mouseUI))
    {
      m_deletePromptVisible = false;
      m_deleteTarget = nullptr;
      return;
    }
  }

  if (leftClick)
  {
    if (m_sellPromptVisible)
    {
      if (m_sellPlus.contains(mouseUI))
      {
        const int maxQty = m_inventory.count(m_sellItemId);
        m_sellQty = std::min(m_sellQty + 1, std::max(1, maxQty));
        updateSellPrompt();
        return;
      }
      if (m_sellMinus.contains(mouseUI))
      {
        m_sellQty = std::max(1, m_sellQty - 1);
        updateSellPrompt();
        return;
      }
      if (m_sellConfirm.contains(mouseUI))
      {
        if (m_inventory.remove(m_sellItemId, m_sellQty))
        {
          const int total = m_sellQty * m_sellPrice;
          m_wallet.addMoney(total);
          updateCurrencyUI();
          m_inventoryPanel.refresh(m_inventory);
          saveGame();
          addPopup({m_uiView.getCenter().x, 18.f}, "+" + std::to_string(total), 2.0f);
        }
        m_sellPromptVisible = false;
        return;
      }
      if (m_sellCancel.contains(mouseUI))
      {
        m_sellPromptVisible = false;
        return;
      }
    }

    if (m_inventoryButton.contains(mouseUI))
    {
      m_inventoryPanel.toggle();
      if (m_inventoryPanel.isOpen())
      {
        if (m_panel.isOpen())
          m_panel.toggle();
        m_inventoryPanel.refresh(m_inventory);
      }
      return;
    }

    if (m_panelButton.contains(mouseUI))
    {
      m_panel.toggle();
      if (m_panel.isOpen() && m_inventoryPanel.isOpen())
        m_inventoryPanel.toggle();
      return;
    }

    if (m_inventoryPanel.isOpen())
    {
      if (auto id = m_inventoryPanel.handleClick(mouseUI))
      {
        showSellPrompt(mouseUI, *id);
        return;
      }
    }

    if (m_panel.handleClick(mouseUI))
    {
      BuildTool newTool = m_panel.selectedTool();

      if (newTool == BuildTool::None)
      {
        m_activeTool = BuildTool::None;
        m_ghost.reset();
        return;
      }

      if (newTool != BuildTool::None)
      {
        m_activeTool = newTool;
        m_ghost = BuildToolFactory::instance()
                      .create(m_activeTool, m_hoverTx, m_hoverTy);
      }
      if (m_ghost)
      {
        m_ghost->x = m_hoverTx;
        m_ghost->y = m_hoverTy;
        m_canPlaceGhost = m_city.canPlace(*m_ghost);
      }

      return;
    }
  }

  if (m_cropMenuVisible && leftClick)
  {
    bool clickedOnButton = false;
    for (size_t i = 0; i < m_cropButtons.size(); ++i)
    {
      if (m_cropButtons[i].contains(mouseUI))
      {
        if (m_selectedCropField)
        {
          static_cast<CropField *>(m_selectedCropField)->plantCrop(m_cropTypes[i].create());
          m_cropMenuVisible = false;
          m_selectedCropField = nullptr;
          saveGame();
        }
        clickedOnButton = true;
        break;
      }
    }
    if (!clickedOnButton)
    {
      m_cropMenuVisible = false;
      m_selectedCropField = nullptr;
    }
    return;
  }

  if ((leftClick || rightClick) &&
      m_activeTool == BuildTool::None &&
      !m_cropMenuVisible &&
      !m_deletePromptVisible)
  {
    auto *p = m_city.getPlaceableAt(m_hoverTx, m_hoverTy);
    if (!p)
    {
      for (const auto &obj : m_city.objects())
      {
        const float left = obj->x * cfg::TileSize;
        const float top = obj->y * cfg::TileSize;
        const float right = (obj->x + obj->w) * cfg::TileSize;
        const float bottom = (obj->y + obj->h) * cfg::TileSize;
        if (mouseWorld.x >= left && mouseWorld.x < right &&
            mouseWorld.y >= top && mouseWorld.y < bottom)
        {
          p = obj.get();
          break;
        }
      }
    }

    if (rightClick)
    {
      if (!p)
        return;
      if (p->canUpgrade(m_city))
      {
        showUpgradePrompt(mouseUI, p);
      }
      else if (p->maxLevel() > 1)
      {
        showToast("Max level reached", 1.2f);
      }
      return;
    }
    if (leftClick && m_exitBtn.getGlobalBounds().contains(mouseUI))
    {
      saveGame();
      m_scenes.set(std::make_unique<MainMenuScreen>(
          m_window,
          m_scenes,
          static_cast<float>(cfg::InternalW),
          static_cast<float>(cfg::InternalH),
          cfg::TileSize));
      return;
    }

    if (leftClick)
    {
      if (p)
      {
        if (auto *c = dynamic_cast<Collectable *>(p))
        {
          CollectResult r = c->collect();
          bool didCollect = false;
          if (r.money > 0)
          {
            m_wallet.addMoney(r.money);
            if (!r.popupText.empty())
              addPopup({(p->x + p->w * 0.5f) * cfg::TileSize,
                        (p->y * cfg::TileSize) - 6.f},
                       r.popupText, 2.0f);
            updateCurrencyUI();
            saveGame();
            didCollect = true;
          }
          if (!r.itemId.empty() && r.itemCount > 0)
          {
            m_inventory.add(r.itemId, r.itemCount);
            m_inventoryPanel.refresh(m_inventory);
            saveGame();
            if (auto *cf = dynamic_cast<CropField *>(p))
            {
              if (cf->crop() == nullptr)
              {
                m_activeTool = BuildTool::None;
                m_ghost.reset();
                m_selectedCropField = p;
                m_cropMenuVisible = true;
                positionCropMenu(mouseUI);
              }
            }
            didCollect = true;
          }
          if (didCollect)
            return;
        }

        if (auto *cf = dynamic_cast<CropField *>(p))
        {
          if (cf->crop() == nullptr)
          {
            m_activeTool = BuildTool::None;
            m_ghost.reset();
            m_selectedCropField = p;
            m_cropMenuVisible = true;
            positionCropMenu(mouseUI);
          }
          return;
        }
      }

      for (const auto &obj : m_city.objects())
      {
        auto *cObj = dynamic_cast<Collectable *>(obj.get());
        if (!cObj || !cObj->canCollect())
          continue;
        auto *pl = dynamic_cast<Placeable *>(obj.get());
        if (!pl)
          continue;

        const auto it = m_collectIcons.find(cObj->collectIconId());
        if (it == m_collectIcons.end())
          continue;

        const float iconSize = 8.f;
        const float cx = (pl->x + pl->w * 0.5f) * cfg::TileSize - iconSize * 0.5f;
        const float cy = (pl->y * cfg::TileSize) - 10.f;
        const sf::FloatRect bounds{{cx, cy}, {iconSize, iconSize}};
        if (bounds.contains(mouseWorld))
        {
          CollectResult r = cObj->collect();
          if (r.money > 0)
          {
            m_wallet.addMoney(r.money);
            if (!r.popupText.empty())
              addPopup({(pl->x + pl->w * 0.5f) * cfg::TileSize,
                        (pl->y * cfg::TileSize) - 6.f},
                       r.popupText, 2.0f);
            updateCurrencyUI();
            saveGame();
          }
          if (!r.itemId.empty() && r.itemCount > 0)
          {
            m_inventory.add(r.itemId, r.itemCount);
            m_inventoryPanel.refresh(m_inventory);
            saveGame();
            if (auto *cf = dynamic_cast<CropField *>(pl))
            {
              if (cf->crop() == nullptr)
              {
                m_activeTool = BuildTool::None;
                m_ghost.reset();
                m_selectedCropField = pl;
                m_cropMenuVisible = true;
                positionCropMenu(mouseUI);
              }
            }
          }
          return;
        }
      }
    }
  }

  if (!m_deletePromptVisible && leftDown &&
      m_activeTool == BuildTool::None && !m_cropMenuVisible)
  {
    auto *p = m_city.getPlaceableAt(m_hoverTx, m_hoverTy);
    if (p)
    {
      if (!p->canBeDeleted())
      {
        showToast("Can't delete this", 1.5f);
        m_holdTarget = nullptr;
        m_holdTime = 0.f;
        return;
      }
      if (p != m_holdTarget)
      {
        m_holdTarget = p;
        m_holdTime = 0.f;
      }
      else
      {
        m_holdTime += dt;
        if (m_holdTime >= m_holdThreshold)
        {
          showDeletePrompt(mouseUI, p);
          m_holdTime = 0.f;
        }
      }
    }
    else
    {
      m_holdTarget = nullptr;
      m_holdTime = 0.f;
    }
  }
  else if (!leftDown)
  {
    m_holdTarget = nullptr;
    m_holdTime = 0.f;
  }

  if (m_ghost)
  {
    m_ghost->x = m_hoverTx;
    m_ghost->y = m_hoverTy;
    m_canPlaceGhost = m_city.canPlace(*m_ghost);
  }

  if (leftClick && m_ghost && m_canPlaceGhost)
  {
    const Cost cost = m_ghost->cost();
    if (!m_wallet.spend(cost))
      return;

    m_city.place(std::move(m_ghost));
    saveGame();

    m_ghost = BuildToolFactory::instance()
                  .create(m_activeTool, m_hoverTx, m_hoverTy);

    if (m_ghost)
      m_canPlaceGhost = m_city.canPlace(*m_ghost);
  }
  else if (leftClick && m_ghost && !m_canPlaceGhost)
  {
    if (auto *b = dynamic_cast<Building *>(m_ghost.get()))
    {
      if (b->requiresRoadAccess() &&
          !m_city.hasRoadAdjacent(m_ghost->x, m_ghost->y, m_ghost->w, m_ghost->h))
      {
        showToast("Build a road first", 2.0f);
      }
    }
  }

  updateCurrencyUI();
}

void BuilderScene::saveGame()
{
  if (!m_saveEnabled)
    return;
  if (m_cityName.empty())
    return;

  const GameState state = SaveSystem::buildState(m_cityName, m_city, m_wallet, m_inventory);
  SaveSystem::saveState(state);
}

void BuilderScene::showToast(const std::string &text, float seconds)
{

  m_toastText.setString(text);
  const sf::FloatRect bounds = m_toastText.getLocalBounds();
  m_toastText.setOrigin({bounds.position.x + bounds.size.x * 0.5f,
                         bounds.position.y + bounds.size.y * 0.5f});
  const sf::Vector2f viewSize = m_uiView.getSize();
  m_toastText.setPosition({viewSize.x * 0.5f, 18.f});
  m_toastTimer = seconds;
}

void BuilderScene::showDeletePrompt(const sf::Vector2f &uiPos, Placeable *target)
{
  if (!target || !target->canBeDeleted())
    return;

  m_deletePromptVisible = true;
  m_deleteTarget = target;

  const Cost sell = target->sellValue();
  if (sell.amount > 0)
  {
    m_deleteMode = DeleteMode::Sell;
    m_deleteLabel->setString("Sell object?");
  }
  else
  {
    m_deleteMode = DeleteMode::Delete;
    m_deleteLabel->setString("Delete object?");
  }

  const sf::Vector2f viewSize = m_uiView.getSize();
  const sf::Vector2f bgSize = m_deleteBg.getSize();
  sf::Vector2f pos = uiPos + sf::Vector2f{8.f, 8.f};
  pos.x = std::clamp(pos.x, 6.f, viewSize.x - bgSize.x - 6.f);
  pos.y = std::clamp(pos.y, 6.f, viewSize.y - bgSize.y - 6.f);
  m_deleteBg.setPosition(pos);

  const sf::FloatRect labelBounds = m_deleteLabel->getLocalBounds();
  m_deleteLabel->setPosition({pos.x + (bgSize.x - labelBounds.size.x) * 0.5f - labelBounds.position.x,
                              pos.y + 6.f});

  const sf::Vector2f yesPos{pos.x + 12.f, pos.y + 26.f};
  const sf::Vector2f noPos{pos.x + bgSize.x - 12.f - 50.f, pos.y + 26.f};

  m_deleteYes = PanelButton({50.f, 18.f}, yesPos);
  m_deleteNo = PanelButton({50.f, 18.f}, noPos);

  m_deleteYes.setOutline(sf::Color(120, 80, 85), 1.f);
  m_deleteNo.setOutline(sf::Color(120, 80, 85), 1.f);

  m_deleteYes.setText(m_uiFont, "Yes", 7, sf::Color::Black);
  m_deleteNo.setText(m_uiFont, "No", 7, sf::Color::Black);

  m_deleteYes.setTextOffset({16.f, 5.f});
  m_deleteNo.setTextOffset({18.f, 5.f});
}

void BuilderScene::addPopup(const sf::Vector2f &worldPos, const std::string &text, float seconds)
{
  PopupText p{sf::Text(m_uiFont, text, 9), worldPos, seconds};
  p.text.setFillColor(sf::Color(255, 230, 180));
  const sf::FloatRect bounds = p.text.getLocalBounds();
  p.text.setOrigin({bounds.position.x + bounds.size.x * 0.5f,
                    bounds.position.y + bounds.size.y * 0.5f});
  m_popups.push_back(p);
}

void BuilderScene::showSellPrompt(const sf::Vector2f &uiPos, const std::string &itemId)
{
  const auto *info = CropRegistry::find(itemId);
  if (!info)
    return;

  m_sellItemId = itemId;
  m_sellPrice = info->sellValue;
  m_sellQty = std::max(1, std::min(m_inventory.count(itemId), 1));
  m_sellPromptVisible = true;

  const sf::Vector2f viewSize = m_uiView.getSize();
  const sf::Vector2f bgSize = m_sellBg.getSize();
  sf::Vector2f pos = uiPos + sf::Vector2f{8.f, 8.f};
  pos.x = std::clamp(pos.x, 6.f, viewSize.x - bgSize.x - 6.f);
  pos.y = std::clamp(pos.y, 6.f, viewSize.y - bgSize.y - 6.f);
  m_sellBg.setPosition(pos);

  m_sellTitle.setString(info->label);
  m_sellTitle.setPosition({std::floor(pos.x + 8.f), std::floor(pos.y + 6.f)});

  const float rowY = pos.y + 24.f;
  m_sellMinus = PanelButton(sf::Vector2f{18.f, 18.f}, {std::floor(pos.x + 8.f), std::floor(rowY)});
  m_sellPlus = PanelButton(sf::Vector2f{18.f, 18.f}, {std::floor(pos.x + 32.f), std::floor(rowY)});

  const float buttonsY = pos.y + 46.f;
  m_sellConfirm = PanelButton(sf::Vector2f{70.f, 18.f}, {std::floor(pos.x + 8.f), std::floor(buttonsY)});
  m_sellCancel = PanelButton(sf::Vector2f{70.f, 18.f}, {std::floor(pos.x + 98.f), std::floor(buttonsY)});

  m_sellPlus.setOutline(sf::Color(120, 80, 85), 1.f);
  m_sellMinus.setOutline(sf::Color(120, 80, 85), 1.f);
  m_sellConfirm.setOutline(sf::Color(120, 80, 85), 1.f);
  m_sellCancel.setOutline(sf::Color(120, 80, 85), 1.f);
  m_sellPlus.setText(m_uiFont, "+", 10, sf::Color::Black);
  m_sellMinus.setText(m_uiFont, "-", 10, sf::Color::Black);
  m_sellConfirm.setText(m_uiFont, "Confirm", 7, sf::Color::Black);
  m_sellCancel.setText(m_uiFont, "Cancel", 7, sf::Color::Black);
  m_sellPlus.setTextOffset({6.f, 3.f});
  m_sellMinus.setTextOffset({6.f, 3.f});
  m_sellConfirm.setTextOffset({10.f, 5.f});
  m_sellCancel.setTextOffset({10.f, 5.f});

  updateSellPrompt();
}

void BuilderScene::updateSellPrompt()
{
  const int maxQty = std::max(1, m_inventory.count(m_sellItemId));
  m_sellQty = std::clamp(m_sellQty, 1, maxQty);
  const int total = m_sellQty * m_sellPrice;

  m_sellQtyText.setString("Qty: " + std::to_string(m_sellQty));
  m_sellPriceText.setString("Total: $" + std::to_string(total));

  const sf::Vector2f pos = m_sellBg.getPosition();
  m_sellQtyText.setPosition({std::floor(pos.x + 60.f), std::floor(pos.y + 28.f)});
  m_sellPriceText.setPosition({std::floor(pos.x + 60.f), std::floor(pos.y + 38.f)});
}

void BuilderScene::render(sf::RenderTarget &target)
{
  m_camera.apply(target);

  m_renderer.drawTiles(target, m_city);

  for (const auto &obj : m_city.objects())
    obj->render(target, m_uiFont);

  const float iconSize = 8.f;
  for (const auto &obj : m_city.objects())
  {
    auto *cObj = dynamic_cast<Collectable *>(obj.get());
    if (!cObj || !cObj->canCollect())
      continue;
    auto *pl = dynamic_cast<Placeable *>(obj.get());
    if (!pl)
      continue;

    auto it = m_collectIcons.find(cObj->collectIconId());
    if (it == m_collectIcons.end())
      continue;

    sf::Sprite icon(it->second);
    const float scale =
        iconSize / static_cast<float>(std::max(it->second.getSize().x, it->second.getSize().y));
    icon.setScale({scale, scale});

    const float cx = (pl->x + pl->w * 0.5f) * cfg::TileSize - iconSize * 0.5f;
    const float cy = (pl->y * cfg::TileSize) - 10.f;
    icon.setPosition({cx, cy});
    target.draw(icon);
  }

  for (auto &p : m_popups)
  {
    p.text.setPosition(p.pos);
    target.draw(p.text);
  }

  if (m_ghost)
  {
    m_ghost->renderGhost(target, m_canPlaceGhost);
  }

  target.setView(m_uiView);
  m_panel.render(target);
  m_inventoryPanel.render(target);
  m_panelButton.draw(target);
  m_inventoryButton.draw(target);
  target.draw(m_coinSprite);
  target.draw(m_diamondSprite);
  target.draw(m_moneyText);
  target.draw(m_diamondText);
  if (m_toastTimer > 0.f)
    target.draw(m_toastText);

  if (m_cropMenuVisible)
  {
    target.draw(m_cropMenuBg);
    for (auto &b : m_cropButtons)
    {
      b.draw(target);
    }
    for (auto &t : m_cropLabels)
    {
      target.draw(t);
    }
  }

  if (m_deletePromptVisible)
  {
    target.draw(m_deleteBg);
    if (m_deleteLabel)
      target.draw(*m_deleteLabel);
    m_deleteYes.draw(target);
    m_deleteNo.draw(target);
  }

  if (m_sellPromptVisible)
  {
    target.draw(m_sellBg);
    target.draw(m_sellTitle);
    target.draw(m_sellQtyText);
    target.draw(m_sellPriceText);
    m_sellPlus.draw(target);
    m_sellMinus.draw(target);
    m_sellConfirm.draw(target);
    m_sellCancel.draw(target);
  }

  if (m_upgradePromptVisible)
  {
    target.draw(m_upgradeBg);
    target.draw(m_upgradeText);
    m_upgradeYes.draw(target);
    m_upgradeNo.draw(target);
  }
  target.draw(m_exitBtn);
  if (m_exitText)
    target.draw(*m_exitText);
}

void BuilderScene::loadFont()
{
  if (!m_uiFont.openFromFile(cfg::FontPixel))
  {
    throw std::runtime_error("BuilderScene: failed to load font");
  }
  m_uiFont.setSmooth(false);
}

void BuilderScene::showUpgradePrompt(const sf::Vector2f &uiPos, Placeable *p)
{
  if (!p || !p->canUpgrade(m_city))
    return;

  m_upgradePromptVisible = true;
  m_upgradeTarget = p;

  const Cost cost = p->upgradeCost();

  m_upgradeBg.setSize({160.f, 60.f});
  m_upgradeBg.setFillColor(sf::Color(40, 40, 40, 220));

  sf::Vector2f pos = uiPos + sf::Vector2f{8.f, 8.f};

  const sf::Vector2f viewSize = m_uiView.getSize();
  const sf::Vector2f size = m_upgradeBg.getSize();

  pos.x = std::clamp(pos.x, 6.f, viewSize.x - size.x - 6.f);
  pos.y = std::clamp(pos.y, 6.f, viewSize.y - size.y - 6.f);

  m_upgradeBg.setPosition(pos);

  m_upgradeText.setFont(m_uiFont);
  m_upgradeText.setCharacterSize(8);
  m_upgradeText.setFillColor(sf::Color::White);
  m_upgradeText.setString(
      "Upgrade to level " + std::to_string(p->level() + 1) +
      "\nCost: " + std::to_string(cost.amount));
  m_upgradeText.setPosition(pos + sf::Vector2f{6.f, 6.f});

  const float btnY = pos.y + size.y - 20.f;

  m_upgradeYes = PanelButton({50.f, 16.f}, {pos.x + 8.f, btnY});
  m_upgradeNo = PanelButton({50.f, 16.f}, {pos.x + size.x - 58.f, btnY});

  m_upgradeYes.setText(m_uiFont, "Upgrade", 7, sf::Color::Black);
  m_upgradeNo.setText(m_uiFont, "Cancel", 7, sf::Color::Black);
}
