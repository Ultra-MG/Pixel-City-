#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "graphics/PixelRenderer.hpp"
#include "engine/SceneManager.hpp"

class Game {
public:
  Game();
  void run();

private:
  sf::RenderWindow m_window;
  PixelRenderer m_pixel;
  SceneManager m_scenes;
};
