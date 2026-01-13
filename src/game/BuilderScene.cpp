#include "game/BuilderScene.hpp"
#include "core/Config.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Mouse.hpp>

#include <iostream>

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

  // UI view matches INTERNAL resolution (critical for text/UI placement)
  m_uiView.setSize((float)cfg::InternalW, (float)cfg::InternalH);
  m_uiView.setCenter((float)cfg::InternalW * 0.5f, (float)cfg::InternalH * 0.5f);

  // Load font (optional). Put it at: project-root/assets/ui.ttf (script copies to dist/assets/ui.ttf)
  m_fontOk = m_font.loadFromFile("assets/ui.ttf");
  if (!m_fontOk) {
    std::cerr << "⚠️ UI font failed to load: assets/ui.ttf\n";
  }
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

  if (m_hoverTx < 0) m_hoverTx = 0;
  if (m_hoverTy < 0) m_hoverTy = 0;
  if (m_hoverTx >= m_city.w()) m_hoverTx = m_city.w() - 1;
  if (m_hoverTy >= m_city.h()) m_hoverTy = m_city.h() - 1;
}

// Smaller buttons (good for InternalW=320..640)
sf::FloatRect BuilderScene::rectConstructButton() const {
  const float w = 60.f;
  const float h = 16.f;
  return sf::FloatRect((float)cfg::InternalW - w - 4.f,
                       (float)cfg::InternalH - h - 4.f,
                       w, h);
}

sf::FloatRect BuilderScene::rectHouseButton() const {
  auto c = rectConstructButton();
  return sf::FloatRect(c.left, c.top - 18.f, c.width, 16.f);
}

sf::FloatRect BuilderScene::rectRoadButton() const {
  auto c = rectConstructButton();
  return sf::FloatRect(c.left, c.top - 36.f, c.width, 16.f);
}

bool BuilderScene::mouseIn(const sf::FloatRect& r, const sf::Vector2f& mouseInternal) const {
  return r.contains(mouseInternal);
}

void BuilderScene::drawButton(sf::RenderTarget& target,
                              const sf::FloatRect& r,
                              bool active,
                              const char* label,
                              sf::Color iconColor) const
{
  sf::RectangleShape bg;
  bg.setPosition(r.left, r.top);
  bg.setSize({r.width, r.height});
  bg.setFillColor(active ? sf::Color(255,255,255,40) : sf::Color(0,0,0,130));
  bg.setOutlineThickness(1.f);
  bg.setOutlineColor(sf::Color(255,255,255,70));
  target.draw(bg);

  // small icon square (always visible even if font fails)
  sf::RectangleShape icon;
  icon.setSize({8.f, 8.f});
  icon.setPosition(r.left + 4.f, r.top + 4.f);
  icon.setFillColor(iconColor);
  target.draw(icon);

  // label text (only if font loaded)
  if (m_fontOk) {
    sf::Text t;
    t.setFont(m_font);
    t.setString(label);
    t.setCharacterSize(9);
    t.setFillColor(sf::Color(255,255,255,230));
    t.setPosition(r.left + 16.f, r.top + 2.f);
    target.draw(t);
  }
}

void BuilderScene::update(float) {
  const sf::Vector2f mouseInternal = windowMouseToInternal();
  updateHoverTile();

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

  // Zoom with wheel
  const float wheel = m_input.wheelDelta();
  if (wheel != 0.f) {
    if (wheel > 0) m_camera.zoom(cfg::ZoomStep);
    else           m_camera.zoom(1.0f / cfg::ZoomStep);
  }

  // --- UI click handling first ---
  const auto rConstruct = rectConstructButton();
  const auto rHouse = rectHouseButton();
  const auto rRoad  = rectRoadButton();

  if (m_input.leftPressed()) {
    if (mouseIn(rConstruct, mouseInternal)) {
      m_constructOpen = !m_constructOpen;
      if (!m_constructOpen) m_tool = BuildTool::None; // back to grass-only
      return;
    }

    if (m_constructOpen && mouseIn(rHouse, mouseInternal)) {
      m_tool = BuildTool::PlaceHouse;
      m_constructOpen = false;
      return;
    }

    if (m_constructOpen && mouseIn(rRoad, mouseInternal)) {
      m_tool = BuildTool::PlaceRoad;
      m_constructOpen = false;
      return;
    }

    // click outside closes + grass-only
    if (m_constructOpen) {
      m_constructOpen = false;
      m_tool = BuildTool::None;
      return;
    }
  }

  // --- Map placement only if tool selected ---
  if (m_input.leftPressed()) {
    if (m_tool == BuildTool::PlaceRoad) {
      // must not paint road over buildings (City::canPlaceRoadAt enforces)
      if (m_city.canPlaceRoadAt(m_hoverTx, m_hoverTy)) {
        m_city.setTile(m_hoverTx, m_hoverTy, Tile::Road);
      }
    } else if (m_tool == BuildTool::PlaceHouse) {
      // must not place on roads + must not overlap buildings (City::canPlaceBuilding enforces)
      Building house;
      house.type = BuildingType::House;
      house.x = m_hoverTx;
      house.y = m_hoverTy;
      house.w = 2;
      house.h = 2;
      house.level = 1;

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

  // House ghost only in house mode
  if (m_tool == BuildTool::PlaceHouse) {
    Building ghost;
    ghost.x = m_hoverTx;
    ghost.y = m_hoverTy;
    ghost.w = 2;
    ghost.h = 2;
    const bool ok = m_city.canPlaceBuilding(ghost);
    m_renderer.drawGhostHouse(target, m_hoverTx, m_hoverTy, ok);
  }

  // UI in INTERNAL coordinates (critical)
  target.setView(m_uiView);

  drawButton(target, rectConstructButton(), false, "Construct", sf::Color(200,200,200,220));

  if (m_constructOpen) {
    drawButton(target, rectRoadButton(),  (m_tool == BuildTool::PlaceRoad),  "Road",  sf::Color(140,140,140,220));
    drawButton(target, rectHouseButton(), (m_tool == BuildTool::PlaceHouse), "House", sf::Color(220,200,120,220));
  }
}