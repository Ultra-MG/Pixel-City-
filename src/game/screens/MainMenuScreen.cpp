#include "game/screens/MainMenuScreen.hpp"
#include "core/Config.hpp"
#include "game/BuilderScene.hpp"
#include "game/screens/NameEntryScreen.hpp"
#include "game/screens/LoadGameScreen.hpp"
#include <SFML/Window/Mouse.hpp>
#include <memory>
#include <stdexcept>
#include <algorithm>

MainMenuScreen::MainMenuScreen(sf::RenderWindow &window,
                             SceneManager &scenes,
                             float internalW,
                             float internalH,
                             int tileSize)
    : m_window(window),
      m_scenes(scenes)
{
    m_uiView.setSize({internalW, internalH});
    m_uiView.setCenter({internalW * 0.5f, internalH * 0.5f});

    if (!m_font.openFromFile(cfg::FontPixel))
    {
        throw std::runtime_error("MainMenuScreen: failed to load font");
    }
    m_font.setSmooth(false);

    m_bgTexture.emplace();
    if (!m_bgTexture->loadFromFile("assets/pixelcity.png"))
    {
        throw std::runtime_error("MainMenuScreen: failed to load assets/pixelcity.png");
    }
    m_bgTexture->setSmooth(false);
    m_bgSprite.emplace(*m_bgTexture);
    const sf::Vector2u bgSize = m_bgTexture->getSize();
    if (bgSize.x > 0 && bgSize.y > 0)
    {
        const float sx = internalW / static_cast<float>(bgSize.x);
        const float sy = internalH / static_cast<float>(bgSize.y);
        m_bgSprite->setScale({sx, sy});
    }

    m_menuItems.clear();
    m_menuBounds.clear();
    m_menuButtons.clear();
    const char *labels[] = {"Create Game", "Load Game", "Settings"};
    const int count = static_cast<int>(sizeof(labels) / sizeof(labels[0]));
    const sf::Vector2f buttonSize{140.f, 24.f};
    const float startY = internalH * 0.5f - 28.f;
    const float centerX = internalW * 0.5f;
    for (int i = 0; i < count; ++i)
    {
        sf::RectangleShape btn(buttonSize);
        btn.setPosition({centerX - buttonSize.x * 0.5f, startY + i * 30.f});
        btn.setFillColor(sf::Color(30, 30, 30, 220));
        btn.setOutlineColor(sf::Color(120, 80, 85));
        btn.setOutlineThickness(1.f);
        m_menuButtons.push_back(btn);

        sf::Text t(m_font, labels[i], 12);
        t.setFillColor(sf::Color(245, 240, 220));
        const sf::FloatRect b = t.getLocalBounds();
        t.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y + b.size.y * 0.5f});
        const sf::Vector2f btnPos = m_menuButtons.back().getPosition();
        t.setPosition({btnPos.x + buttonSize.x * 0.5f, btnPos.y + buttonSize.y * 0.5f - 1.f});
        m_menuItems.push_back(t);
        m_menuBounds.push_back(t.getGlobalBounds());
    }

    if (!m_menuBounds.empty())
    {
        float left = m_menuButtons[0].getPosition().x;
        float top = m_menuButtons[0].getPosition().y;
        float right = left + m_menuButtons[0].getSize().x;
        float bottom = top + m_menuButtons[0].getSize().y;
        for (size_t i = 1; i < m_menuButtons.size(); ++i)
        {
            const sf::Vector2f pos = m_menuButtons[i].getPosition();
            const sf::Vector2f size = m_menuButtons[i].getSize();
            left = std::min(left, pos.x);
            top = std::min(top, pos.y);
            right = std::max(right, pos.x + size.x);
            bottom = std::max(bottom, pos.y + size.y);
        }
        const float padX = 12.f;
        const float padY = 10.f;
        m_menuBg.setPosition({left - padX, top - padY});
        m_menuBg.setSize({(right - left) + padX * 2.f, (bottom - top) + padY * 2.f});
        m_menuBg.setFillColor(sf::Color(0, 0, 0, 120));
        m_menuBg.setOutlineColor(sf::Color(255, 255, 255, 40));
        m_menuBg.setOutlineThickness(1.f);
    }
}

void MainMenuScreen::handleEvent(const sf::Event &e)
{
    if (!e.is<sf::Event::MouseButtonPressed>())
        return;

    const auto mouse = m_window.mapPixelToCoords(
        sf::Mouse::getPosition(m_window), m_uiView);

    for (size_t i = 0; i < m_menuBounds.size(); ++i)
    {
        if (!m_menuBounds[i].contains(mouse))
            continue;

        if (i == 0)
        {
            m_scenes.set(std::make_unique<NameEntryScreen>(
                m_window,
                m_scenes,
                static_cast<float>(cfg::InternalW),
                static_cast<float>(cfg::InternalH),
                cfg::TileSize));
            return;
        }
        if (i == 1)
        {
            m_scenes.set(std::make_unique<LoadGameScreen>(
                m_window,
                m_scenes,
                static_cast<float>(cfg::InternalW),
                static_cast<float>(cfg::InternalH),
                cfg::TileSize));
            return;
        }
        return;
    }
}

void MainMenuScreen::update(float)
{
}

void MainMenuScreen::render(sf::RenderTarget &target)
{
    target.setView(m_uiView);
    if (m_bgSprite)
        target.draw(*m_bgSprite);
    target.draw(m_menuBg);
    for (auto &b : m_menuButtons)
        target.draw(b);
    for (auto &t : m_menuItems)
        target.draw(t);
}
