#pragma once

#include <SFML/Graphics.hpp>

class Scene {
public:
  virtual ~Scene() = default;

  virtual void beginFrame() {}

  virtual void handleEvent(const sf::Event& e) = 0;
  virtual void update(float dt) = 0;
  virtual void render(sf::RenderTarget& target) = 0;
};
