// src/game/BuilderScene.cpp  (FIXED)

#include "game/BuilderScene.hpp"
#include "core/Config.hpp"
#include "world/House.hpp"
#include "world/Road.hpp"
#include "world/Fountain.hpp"
#include <iostream>
#include "game/BuildToolFactory.hpp"
#include <algorithm>
#include <SFML/Window/Mouse.hpp>

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

  m_panelButton = Button({20.f, 20.f}, {6.f, 6.f});
  m_panelButton.setOutline(sf::Color(70, 70, 75), 1.f);

  m_panelButton.loadImage("assets/ui/build_button.png");
  m_panelButton.setImageScale({0.2f, 0.2f});
  m_panelButton.setImageOffset({2.f, 2.f});

  House::loadTexture();
  Road::loadTexture();
  Fountain::loadTexture();
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
}
