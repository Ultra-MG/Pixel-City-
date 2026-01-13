#include "engine/Input.hpp"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>

void Input::beginFrame() {
  m_leftPressed = false;
  m_rightPressed = false;
  m_wheelDelta = 0.f;

  m_tool1Pressed = false;
  m_tool2Pressed = false;
  m_tool3Pressed = false;

  m_dragDelta = {0,0};
  m_draggingRight = false;
}

void Input::handleEvent(const sf::Event& e) {
  if (e.type == sf::Event::MouseButtonPressed) {
    if (e.mouseButton.button == sf::Mouse::Left) m_leftPressed = true;
    if (e.mouseButton.button == sf::Mouse::Right) { m_rightPressed = true; m_rightDown = true; }
  }

  if (e.type == sf::Event::MouseButtonReleased) {
    if (e.mouseButton.button == sf::Mouse::Right) m_rightDown = false;
  }

  if (e.type == sf::Event::MouseWheelScrolled) {
    // positive = wheel up
    m_wheelDelta += e.mouseWheelScroll.delta;
  }

  if (e.type == sf::Event::KeyPressed) {
    if (e.key.code == sf::Keyboard::Num1) m_tool1Pressed = true;
    if (e.key.code == sf::Keyboard::Num2) m_tool2Pressed = true;
    if (e.key.code == sf::Keyboard::Num3) m_tool3Pressed = true;
  }

  // Right-drag panning
  if (e.type == sf::Event::MouseMoved) {
    const sf::Vector2i now { e.mouseMove.x, e.mouseMove.y };
    if (m_rightDown) {
      m_draggingRight = true;
      m_dragDelta = now - m_lastMouse;
    }
    m_lastMouse = now;
  }
}
