#pragma once
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>

class Input {
public:
  void beginFrame();
  void handleEvent(const sf::Event& e);

  // one-frame presses
  bool leftPressed() const { return m_leftPressed; }
  bool rightPressed() const { return m_rightPressed; }

  // held
  bool rightDown() const { return m_rightDown; }

  // mouse wheel delta this frame (can be 0)
  float wheelDelta() const { return m_wheelDelta; }

  // keys: tool switching
  bool tool1Pressed() const { return m_tool1Pressed; }
  bool tool2Pressed() const { return m_tool2Pressed; }
  bool tool3Pressed() const { return m_tool3Pressed; }

  // drag
  bool draggingRight() const { return m_draggingRight; }
  sf::Vector2i dragDelta() const { return m_dragDelta; }

private:
  bool m_leftPressed = false;
  bool m_rightPressed = false;

  bool m_rightDown = false;

  float m_wheelDelta = 0.f;

  bool m_tool1Pressed = false;
  bool m_tool2Pressed = false;
  bool m_tool3Pressed = false;

  bool m_draggingRight = false;
  sf::Vector2i m_lastMouse {0,0};
  sf::Vector2i m_dragDelta {0,0};
};
