#include "game/screens/SplashScreen.hpp"
#include "core/Config.hpp"
#include "game/screens/MainMenuScreen.hpp"
#include <memory>
#include <stdexcept>
#include <algorithm>

SplashScreen::SplashScreen(sf::RenderWindow &window,
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
        throw std::runtime_error("SplashScreen: failed to load font");
    }
    m_font.setSmooth(false);

    m_title.emplace(m_font, "PIXELCITY", 28);
    m_title->setFillColor(sf::Color(245, 240, 220));
    const sf::FloatRect titleBounds = m_title->getLocalBounds();
    m_title->setOrigin({titleBounds.position.x + titleBounds.size.x * 0.5f,
                        titleBounds.position.y + titleBounds.size.y * 0.5f});
    m_title->setPosition({internalW * 0.5f, internalH * 0.35f});

    m_hint.emplace(m_font, "Click Anywhere to Continue", 10);
    m_hint->setFillColor(sf::Color(200, 200, 200));
    const sf::FloatRect hintBounds = m_hint->getLocalBounds();
    m_hint->setOrigin({hintBounds.position.x + hintBounds.size.x * 0.5f,
                       hintBounds.position.y + hintBounds.size.y * 0.5f});
    m_hint->setPosition({internalW * 0.5f, internalH * 0.62f});

    if (m_title && m_hint)
    {
        const sf::FloatRect titleG = m_title->getGlobalBounds();
        const sf::FloatRect hintG = m_hint->getGlobalBounds();
        const float left = std::min(titleG.position.x, hintG.position.x);
        const float top = std::min(titleG.position.y, hintG.position.y);
        const float right = std::max(titleG.position.x + titleG.size.x,
                                     hintG.position.x + hintG.size.x);
        const float bottom = std::max(titleG.position.y + titleG.size.y,
                                      hintG.position.y + hintG.size.y);

        const float padX = 14.f;
        const float padY = 10.f;
        m_textBg.setPosition({left - padX, top - padY});
        m_textBg.setSize({(right - left) + padX * 2.f, (bottom - top) + padY * 2.f});
        m_textBg.setFillColor(sf::Color(0, 0, 0, 130));
        m_textBg.setOutlineColor(sf::Color(255, 255, 255, 40));
        m_textBg.setOutlineThickness(1.f);
    }


    m_bgTexture.emplace();
    if (!m_bgTexture->loadFromFile("assets/pixelcity.png"))
    {
        throw std::runtime_error("SplashScreen: failed to load assets/pixelcity.png");
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
}

void SplashScreen::handleEvent(const sf::Event &e)
{
    if (!e.is<sf::Event::MouseButtonPressed>())
        return;

    m_scenes.set(std::make_unique<MainMenuScreen>(
        m_window,
        m_scenes,
        static_cast<float>(cfg::InternalW),
        static_cast<float>(cfg::InternalH),
        cfg::TileSize));
}

void SplashScreen::update(float dt)
{
    m_blinkTimer += dt;
    if (m_blinkTimer >= 0.5f)
    {
        m_blinkTimer = 0.f;
        m_showHint = !m_showHint;
    }
}

void SplashScreen::render(sf::RenderTarget &target)
{
    target.setView(m_uiView);
    if (m_bgSprite)
        target.draw(*m_bgSprite);
    target.draw(m_textBg);
    if (m_title)
        target.draw(*m_title);
    if (m_showHint)
        if (m_hint)
            target.draw(*m_hint);
}
