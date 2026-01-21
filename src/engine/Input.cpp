#include "engine/Input.hpp"

void Input::beginFrame() {
  m_leftPressed = false;
  m_rightPressed = false;
  m_wheelDelta = 0.f;

  m_tool1Pressed = false;
  m_tool2Pressed = false;
  m_tool3Pressed = false;

  m_draggingRight = false;
  m_dragDelta = {0, 0};
}

void Input::handleEvent(const sf::Event& e) {

  if (const auto* mb = e.getIf<sf::Event::MouseButtonPressed>()) {
    if (mb->button == sf::Mouse::Button::Left)
      m_leftPressed = true;

    if (mb->button == sf::Mouse::Button::Right) {
      m_rightPressed = true;
      m_rightDown = true;
    }
  }

  if (const auto* mb = e.getIf<sf::Event::MouseButtonReleased>()) {
    if (mb->button == sf::Mouse::Button::Right)
      m_rightDown = false;
  }

  if (const auto* mw = e.getIf<sf::Event::MouseWheelScrolled>()) {
    m_wheelDelta += mw->delta;
  }

  if (const auto* k = e.getIf<sf::Event::KeyPressed>()) {
    if (k->code == sf::Keyboard::Key::Num1) m_tool1Pressed = true;
    if (k->code == sf::Keyboard::Key::Num2) m_tool2Pressed = true;
    if (k->code == sf::Keyboard::Key::Num3) m_tool3Pressed = true;
  }

  if (const auto* mm = e.getIf<sf::Event::MouseMoved>()) {
    sf::Vector2i now(mm->position.x, mm->position.y);

    if (m_rightDown) {
      m_draggingRight = true;
      m_dragDelta = now - m_lastMouse;
    }

    m_lastMouse = now;
  }
}
