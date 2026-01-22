#include "game/BuilderScene.hpp"
#include "core/Config.hpp"
#include "world/Buildings/House.hpp"
#include "world/Buildings/Farm.hpp"
#include "world/Buildings/Store.hpp"
#include "world/Buildings/TownHall.hpp"
#include "world/Infrastructure/Road.hpp"
#include "world/Infrastructure/Water.hpp"
#include "world/Infrastructure/Bridge.hpp"
#include "world/Infrastructure/CropField.hpp"
#include "world/Crops/Wheat.hpp"
#include "world/Crops/Corn.hpp"
#include "world/Crops/Carrot.hpp"
#include "world/Crops/Potato.hpp"
#include "world/Decorations/Fountain.hpp"
#include "world/Decorations/Tree.hpp"
#include "world/Decorations/Bench.hpp"
#include "world/Decorations/LampPost.hpp"
#include "game/BuildToolFactory.hpp"
#include <SFML/Window/Mouse.hpp>
#include <stdexcept>

BuilderScene::BuilderScene(sf::RenderWindow &window,
                           float internalW,
                           float internalH,
                           int tileSize)
    : m_window(window), m_camera(internalW, internalH), m_city(cfg::CityW, cfg::CityH),
      m_renderer(tileSize, "assets/grass.png"), m_panel(40.f, 160.f, internalH)
{
  m_camera.setZoomClamp(cfg::ZoomMin, cfg::ZoomMax);

  const float mapW = float(cfg::CityW * cfg::TileSize);
  const float mapH = float(cfg::CityH * cfg::TileSize);
  m_camera.setCenter({mapW * 0.5f, mapH * 0.5f});

  m_uiView.setSize({internalW, internalH});
  m_uiView.setCenter({internalW * 0.5f, internalH * 0.5f});

  if (!m_uiFont.openFromFile("assets/fonts/pixelFont.ttf"))
  {
    throw std::runtime_error("BuilderScene: failed to load font");
  }

  m_cropMenuBg.setSize({170.f, 140.f});
  m_cropMenuBg.setFillColor(sf::Color(30, 30, 30, 220));
  m_cropMenuBg.setOutlineColor(sf::Color(120, 80, 85));
  m_cropMenuBg.setOutlineThickness(1.f);
  m_cropMenuBg.setPosition({0.f, 0.f});

  m_panelButton = Button({20.f, 20.f}, {6.f, 6.f});
  m_panelButton.setOutline(sf::Color(70, 70, 75), 1.f);

  m_panelButton.loadImage("assets/ui/build_button.png");
  m_panelButton.setImageScale({0.2f, 0.2f});
  m_panelButton.setImageOffset({2.f, 2.f});

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

  // Initialize crop types
  m_cropTypes.emplace_back(CropType{"Wheat", [](){ return new Wheat(); }, "assets/crops/wheat.png"});
  m_cropTypes.emplace_back(CropType{"Corn", [](){ return new Corn(); }, "assets/crops/corn.png"});
  m_cropTypes.emplace_back(CropType{"Carrot", [](){ return new Carrot(); }, "assets/crops/carrot.png"});
  m_cropTypes.emplace_back(CropType{"Potato", [](){ return new Potato(); }, "assets/crops/potato.png"});

  // Initialize crop buttons
  const sf::Vector2f buttonSize{40.f, 40.f};
  for (size_t i = 0; i < m_cropTypes.size(); ++i) {
      m_cropButtons.emplace_back(Button(buttonSize, {0.f, 0.f}));
      m_cropButtons.back().loadImage(m_cropTypes[i].iconPath);
      m_cropButtons.back().setImageSize({32.f, 32.f});
      m_cropButtons.back().setImageOffset({4.f, 4.f});
      m_cropLabels.emplace_back(m_uiFont, m_cropTypes[i].name, 10);
      m_cropLabels.back().setFillColor(sf::Color::White);
  }
}

// ------------------------------------------------------------

void BuilderScene::beginFrame()
{
  m_input.beginFrame();
}

void BuilderScene::handleEvent(const sf::Event &e)
{
  m_input.handleEvent(e);
}

// ------------------------------------------------------------

sf::Vector2f BuilderScene::windowMouseToInternal() const
{
  const sf::Vector2i mp = sf::Mouse::getPosition(m_window);
  return {
      float(mp.x) / float(cfg::Scale),
      float(mp.y) / float(cfg::Scale)};
}

// ------------------------------------------------------------

void BuilderScene::updateHoverTile()
{
  const sf::Vector2f mouseInternal = windowMouseToInternal();
  const sf::Vector2f world = m_camera.screenToWorld(mouseInternal);

  m_hoverTx = std::clamp(int(world.x) / cfg::TileSize, 0, m_city.w() - 1);
  m_hoverTy = std::clamp(int(world.y) / cfg::TileSize, 0, m_city.h() - 1);
}

// ------------------------------------------------------------

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

// ------------------------------------------------------------

void BuilderScene::update(float)
{

  updateHoverTile();

  if (m_panel.isOpen())
  {
    m_panelButton.loadImage("assets/ui/close.png");
    m_panelButton.setImageSize({16.f, 16.f});
  }
  else
  {
    m_panelButton.loadImage("assets/ui/build_button.png");
    m_panelButton.setImageScale({0.2f, 0.2f});
  }

  const sf::Vector2i mp = sf::Mouse::getPosition(m_window);
  const sf::Vector2f mouseUI =
      m_window.mapPixelToCoords(mp, m_uiView);

  const bool click = m_input.leftPressed();

  if (click)
  {

    if (m_panelButton.contains(mouseUI))
    {
      m_panel.toggle();
      return;
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

  // =========================
  // Crop menu handling
  // =========================
  if (m_cropMenuVisible && click) {
      bool clickedOnButton = false;
      for (size_t i = 0; i < m_cropButtons.size(); ++i) {
          if (m_cropButtons[i].contains(mouseUI)) {
              if (m_selectedCropField) {
                  static_cast<CropField*>(m_selectedCropField)->plantCrop(m_cropTypes[i].create());
                  m_cropMenuVisible = false;
                  m_selectedCropField = nullptr;
              }
              clickedOnButton = true;
              break;
          }
      }
      if (!clickedOnButton) {
          m_cropMenuVisible = false;
          m_selectedCropField = nullptr;
      }
      return;
  }

  if (click && m_activeTool == BuildTool::None && !m_cropMenuVisible) {
      auto p = m_city.getPlaceableAt(m_hoverTx, m_hoverTy);
      if (p && dynamic_cast<CropField*>(p)) {
          m_selectedCropField = p;
          m_cropMenuVisible = true;
          positionCropMenu(mouseUI);
          return;
      }
  }

  // =========================
  // Ghost update (world)
  // =========================
  if (m_ghost)
  {
    m_ghost->x = m_hoverTx;
    m_ghost->y = m_hoverTy;
    m_canPlaceGhost = m_city.canPlace(*m_ghost);
  }

  // =========================
  // Place object (world)
  // =========================
  if (click && m_ghost && m_canPlaceGhost)
  {
    m_city.place(std::move(m_ghost));
    m_ghost = BuildToolFactory::instance()
                  .create(m_activeTool, m_hoverTx, m_hoverTy);

    if (m_ghost)
      m_canPlaceGhost = m_city.canPlace(*m_ghost);
  }
}

// ------------------------------------------------------------

void BuilderScene::render(sf::RenderTarget &target)
{
  m_camera.apply(target);

  // tiles
  m_renderer.drawTiles(target, m_city);

  // placed objects
  for (const auto &obj : m_city.objects())
    obj->render(target);

  // ghost
  if (m_ghost)
  {
    m_ghost->renderGhost(target, m_canPlaceGhost);
  }

  // UI
  target.setView(m_uiView);
  m_panel.render(target);
  m_panelButton.draw(target);

  if (m_cropMenuVisible) {
      target.draw(m_cropMenuBg);
      for (auto& b : m_cropButtons) {
          b.draw(target);
      }
      for (auto& t : m_cropLabels) {
          target.draw(t);
      }
  }
}
