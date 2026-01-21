#include "core/Game.hpp"
#include "core/Config.hpp"
#include "game/BuilderScene.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "game/BuildToolFactory.hpp"
#include "world/House.hpp"
#include "world/Road.hpp"
#include "world/Fountain.hpp"

Game::Game()
    : m_window(
          sf::VideoMode({cfg::WindowW, cfg::WindowH}),
          "PixelCity Builder",
          sf::Style::Titlebar | sf::Style::Close),
      m_pixel(cfg::InternalW, cfg::InternalH, cfg::Scale)
{
  m_window.setVerticalSyncEnabled(true);

  m_scenes.set(std::make_unique<BuilderScene>(
      m_window,
      static_cast<float>(cfg::InternalW),
      static_cast<float>(cfg::InternalH),
      cfg::TileSize));
  auto &factory = BuildToolFactory::instance();

  factory.registerTool(BuildTool::PlaceHouse,
                       [](int x, int y)
                       { return std::make_unique<House>(x, y); });

  factory.registerTool(BuildTool::PlaceRoad,
                       [](int x, int y)
                       { return std::make_unique<Road>(x, y); });

  factory.registerTool(BuildTool::PlaceFountain,
                       [](int x, int y)
                       { return std::make_unique<Fountain>(x, y); });
}

void Game::run()
{
  sf::Clock clock;

  while (m_window.isOpen())
  {
    const float dt = clock.restart().asSeconds();

    m_scenes.current().beginFrame();

    while (auto event = m_window.pollEvent())
    {
      if (event->is<sf::Event::Closed>())
      {
        m_window.close();
        continue;
      }

      m_scenes.current().handleEvent(*event);
    }

    m_scenes.current().update(dt);

    m_window.clear();
    m_pixel.begin();
    m_scenes.current().render(m_pixel.target());
    m_pixel.endAndPresent(m_window);
    m_window.display();
  }
}
