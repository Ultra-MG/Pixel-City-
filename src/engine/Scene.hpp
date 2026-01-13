#pragma once
#include <SFML/Window/Event.hpp>

namespace sf { class RenderTarget; }

class Scene {
public:
  virtual ~Scene() = default;

  // Called once per frame BEFORE events are polled.
  // Use this to reset one-frame inputs (pressed flags, wheel delta, etc.)
  virtual void beginFrame() {}

  virtual void handleEvent(const sf::Event& e) = 0;
  virtual void update(float dt) = 0;
  virtual void render(sf::RenderTarget& target) = 0;
};
