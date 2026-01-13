#include "game/BuilderScene.hpp"
#include "core/Config.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Mouse.hpp>

#include <iostream>
#include <cmath>

static float len2(float x, float y) { return x * x + y * y; }

BuilderScene::BuilderScene(sf::RenderWindow& window, float internalW, float internalH, int tileSize)
: m_window(window)
, m_camera(internalW, internalH)
, m_city(cfg::CityW, cfg::CityH)
, m_renderer(tileSize, "assets/grass.png", "assets/road.png", "assets/house.png")
{
  m_camera.setZoomClamp(cfg::ZoomMin, cfg::ZoomMax);

  // Center camera on map
  const float mapW = (float)(cfg::CityW * cfg::TileSize);
  const float mapH = (float)(cfg::CityH * cfg::TileSize);
  m_camera.setCenter({ mapW * 0.5f, mapH * 0.5f });

  // UI view matches INTERNAL resolution
  m_uiView.setSize((float)cfg::InternalW, (float)cfg::InternalH);
  m_uiView.setCenter((float)cfg::InternalW * 0.5f, (float)cfg::InternalH * 0.5f);

  m_fontOk = m_font.loadFromFile("assets/ui.ttf");
  if (!m_fontOk) {
    std::cerr << "⚠️ UI font failed to load: assets/ui.ttf\n";
  }

  // ----------------------------
  // SMALL wheel bottom-right
  // ----------------------------
  RadialMenu::Style st;
  st.innerRadius = 7.f;    // was 4
  st.outerRadius = 20.f;   // was 12
  st.iconSize    = 10.f;   // was 6
  st.arcSteps    = 10;
  m_wheel.setStyle(st);

  // Slightly more margin from corner
  m_wheel.setCenter({
    (float)cfg::InternalW - 26.f,
    (float)cfg::InternalH - 26.f
  });

  // Icons optional (placeholders will show if missing, because RadialMenu creates them)
  m_wheel.clearItems();
  m_wheel.addItem(1, "assets/icon_road.png", true);
  m_wheel.addItem(2, "assets/icon_house.png", true);

  m_constructOpen = false;
  m_wheel.setOpen(false);
}

void BuilderScene::beginFrame() { m_input.beginFrame(); }
void BuilderScene::handleEvent(const sf::Event& e) { m_input.handleEvent(e); }

sf::Vector2f BuilderScene::windowMouseToInternal() const {
  const auto mp = sf::Mouse::getPosition(m_window);
  return sf::Vector2f(
    (float)mp.x / (float)cfg::Scale,
    (float)mp.y / (float)cfg::Scale
  );
}

void BuilderScene::updateHoverTile() {
  const sf::Vector2f mouseInternal = windowMouseToInternal();
  const sf::Vector2f world = m_camera.screenToWorld(mouseInternal);

  m_hoverTx = (int)(world.x) / cfg::TileSize;
  m_hoverTy = (int)(world.y) / cfg::TileSize;

  // ✅ Make houses (2x2) always stay in bounds when selected
  int maxX = m_city.w() - 1;
  int maxY = m_city.h() - 1;
  if (m_tool == BuildTool::PlaceHouse) {
    maxX = m_city.w() - 2;
    maxY = m_city.h() - 2;
  }
  if (maxX < 0) maxX = 0;
  if (maxY < 0) maxY = 0;

  if (m_hoverTx < 0) m_hoverTx = 0;
  if (m_hoverTy < 0) m_hoverTy = 0;
  if (m_hoverTx > maxX) m_hoverTx = maxX;
  if (m_hoverTy > maxY) m_hoverTy = maxY;
}

void BuilderScene::update(float) {
  updateHoverTile();

  // UI mouse coords (robust)
  const sf::Vector2i mp = sf::Mouse::getPosition(m_window);
  const sf::Vector2f mouseUI = m_window.mapPixelToCoords(mp, m_uiView);

  // Pan camera with right-drag
  if (m_input.draggingRight()) {
    const sf::Vector2i dWin = m_input.dragDelta();
    const sf::Vector2f dInternal(
      (float)dWin.x / (float)cfg::Scale,
      (float)dWin.y / (float)cfg::Scale
    );
    const float z = m_camera.zoomLevel();
    m_camera.move(sf::Vector2f(-dInternal.x / z, -dInternal.y / z));
  }

  // Zoom
  const float wheel = m_input.wheelDelta();
  if (wheel != 0.f) {
    if (wheel > 0) m_camera.zoom(cfg::ZoomStep);
    else           m_camera.zoom(1.0f / cfg::ZoomStep);
  }

  // Ghost validity
  m_canPlaceGhostRoad = m_city.canPlaceRoadAt(m_hoverTx, m_hoverTy);

  Building ghostHouse;
  ghostHouse.type  = BuildingType::House;
  ghostHouse.x     = m_hoverTx;
  ghostHouse.y     = m_hoverTy;
  ghostHouse.w     = 2;
  ghostHouse.h     = 2;
  ghostHouse.level = 1;
  m_canPlaceGhostHouse = m_city.canPlaceBuilding(ghostHouse);

  // ----------------------------------------------------
  // ✅ Click hitboxes scale with wheel size (fix)
  // ----------------------------------------------------
  if (m_input.leftPressed()) {
    const sf::Vector2f c = m_wheel.center();
    const float dx = mouseUI.x - c.x;
    const float dy = mouseUI.y - c.y;
    const float d2 = len2(dx, dy);

    const float rInner = m_wheel.style().innerRadius;
    const float rOuter = m_wheel.style().outerRadius;

    // Small proportional assistance (NOT a fixed +8)
    const float help = 1.25f;

    const float centerHit = rInner * help;
    const float ringInner = rInner * help;
    const float ringOuter = rOuter * help;

    const float centerHit2 = centerHit * centerHit;
    const float ringInner2 = ringInner * ringInner;
    const float ringOuter2 = ringOuter * ringOuter;

    // Center click toggles wheel
    if (d2 <= centerHit2) {
      m_constructOpen = !m_constructOpen;
      if (!m_constructOpen) m_tool = BuildTool::None;
      return;
    }

    // If open and click inside ring:
    // TOP half => Road, BOTTOM half => House
    if (m_constructOpen) {
      if (d2 >= ringInner2 && d2 <= ringOuter2) {
        if (dy < 0.f) m_tool = BuildTool::PlaceRoad;
        else          m_tool = BuildTool::PlaceHouse;

        m_constructOpen = false;
        return;
      }

      // click outside ring closes
      m_constructOpen = false;
      m_tool = BuildTool::None;
      return;
    }
  }

  // Map placement
  if (m_input.leftPressed()) {
    if (m_tool == BuildTool::PlaceRoad) {
      if (m_canPlaceGhostRoad) {
        m_city.setTile(m_hoverTx, m_hoverTy, Tile::Road);
      }
    } else if (m_tool == BuildTool::PlaceHouse) {
      Building house = ghostHouse;
      if (m_city.canPlaceBuilding(house)) {
        m_city.placeBuilding(house);
      }
    }
  }
}

void BuilderScene::render(sf::RenderTarget& target) {
  // World
  m_camera.apply(target);
  m_renderer.draw(target, m_city);
  m_renderer.drawTileHover(target, m_hoverTx, m_hoverTy);

  // Ghosts
  if (m_tool == BuildTool::PlaceRoad) {
    m_renderer.drawGhostRoad(target, m_hoverTx, m_hoverTy, m_canPlaceGhostRoad);
  }
  if (m_tool == BuildTool::PlaceHouse) {
    m_renderer.drawGhostHouse(target, m_hoverTx, m_hoverTy, m_canPlaceGhostHouse);
  }

  // UI
  target.setView(m_uiView);

  // Always draw center button
  const sf::Vector2f c = m_wheel.center();
  const float rInner = m_wheel.style().innerRadius;

  sf::CircleShape center;
  center.setRadius(rInner);
  center.setOrigin(rInner, rInner);
  center.setPosition(c);
  center.setFillColor(sf::Color(0, 0, 0, 190));
  center.setOutlineThickness(1.f);
  center.setOutlineColor(sf::Color(255, 0, 0, 220));
  target.draw(center);

  // Draw wheel only when open
  if (m_constructOpen) {
    const sf::Vector2i mp = sf::Mouse::getPosition(m_window);
    const sf::Vector2f mouseUI = m_window.mapPixelToCoords(mp, m_uiView);

    m_wheel.setOpen(true);
    m_wheel.draw(target, mouseUI);
    m_wheel.setOpen(false);
  }
}
