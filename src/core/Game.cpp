#include "core/Game.hpp"
#include "core/Config.hpp"
#include "game/BuilderScene.hpp"
#include "game/screens/SplashScreen.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "game/BuildToolFactory.hpp"
#include "world/Buildings/House.hpp"
#include "world/Buildings/Farm.hpp"
#include "world/Buildings/Store.hpp"
#include "world/Buildings/TownHall.hpp"
#include "world/Infrastructure/Road.hpp"
#include "world/Infrastructure/Water.hpp"
#include "world/Infrastructure/Bridge.hpp"
#include "world/Infrastructure/CropField.hpp"
#include "world/Decorations/Fountain.hpp"
#include "world/Decorations/Tree.hpp"
#include "world/Decorations/Bench.hpp"
#include "world/Decorations/LampPost.hpp"

Game::Game()
    : m_window(
          sf::VideoMode({cfg::WindowW, cfg::WindowH}),
          "PixelCity Builder",
          sf::Style::Titlebar | sf::Style::Close),
      m_pixel(cfg::InternalW, cfg::InternalH, cfg::Scale)
{
  m_window.setVerticalSyncEnabled(true);

  m_scenes.set(std::make_unique<SplashScreen>(
      m_window,
      m_scenes,
      static_cast<float>(cfg::InternalW),
      static_cast<float>(cfg::InternalH),
      cfg::TileSize));
  auto &factory = BuildToolFactory::instance();

  factory.registerTool(BuildTool::PlaceHouse,
                       [](int x, int y)
                       { return std::make_unique<House>(x, y); });

  factory.registerTool(BuildTool::PlaceStore,
                       [](int x, int y)
                       { return std::make_unique<Store>(x, y); });

  factory.registerTool(BuildTool::PlaceFarm,
                       [](int x, int y)
                       { return std::make_unique<Farm>(x, y); });

  factory.registerTool(BuildTool::PlaceRoad,
                       [](int x, int y)
                       { return std::make_unique<Road>(x, y); });

  factory.registerTool(BuildTool::PlaceWater,
                       [](int x, int y)
                       { return std::make_unique<Water>(x, y); });

  factory.registerTool(BuildTool::PlaceBridge,
                       [](int x, int y)
                       { return std::make_unique<Bridge>(x, y); });

  factory.registerTool(BuildTool::PlaceCropField,
                       [](int x, int y)
                       { return std::make_unique<CropField>(x, y); });

  factory.registerTool(BuildTool::PlaceTownHall,
                       [](int x, int y)
                       { return std::make_unique<TownHall>(x, y); });

  factory.registerTool(BuildTool::PlaceFountain,
                       [](int x, int y)
                       { return std::make_unique<Fountain>(x, y); });
  factory.registerTool(BuildTool::PlaceTree,
                       [](int x, int y)
                       { return std::make_unique<Tree>(x, y); });
  factory.registerTool(BuildTool::PlaceBench,
                       [](int x, int y)
                       { return std::make_unique<Bench>(x, y); });
  factory.registerTool(BuildTool::PlaceLampPost,
                       [](int x, int y)
                       { return std::make_unique<LampPost>(x, y); });
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
