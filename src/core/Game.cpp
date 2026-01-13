#include "core/Game.hpp"
#include "core/Config.hpp"
#include "game/BuilderScene.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>

Game::Game()
: m_window(sf::VideoMode(cfg::WindowW, cfg::WindowH),
           "PixelCity Builder",
           sf::Style::Titlebar | sf::Style::Close)
, m_pixel(cfg::InternalW, cfg::InternalH, cfg::Scale)
{
  m_window.setVerticalSyncEnabled(true);

  m_scenes.set(std::make_unique<BuilderScene>(
    m_window,
    static_cast<float>(cfg::InternalW),
    static_cast<float>(cfg::InternalH),
    cfg::TileSize
  ));
}

void Game::run() {
  sf::Clock clock;

  while (m_window.isOpen()) {
    const float dt = clock.restart().asSeconds();

    // ✅ IMPORTANT: reset "pressed this frame" flags BEFORE polling events
    m_scenes.current().beginFrame();

    sf::Event e;
    while (m_window.pollEvent(e)) {
      if (e.type == sf::Event::Closed) m_window.close();
      m_scenes.current().handleEvent(e);
    }

    m_scenes.current().update(dt);

    m_window.clear();
    m_pixel.begin();
    m_scenes.current().render(m_pixel.target());
    m_pixel.endAndPresent(m_window);
    m_window.display();
  }
}
