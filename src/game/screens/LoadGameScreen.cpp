#include "game/screens/LoadGameScreen.hpp"
#include "game/BuilderScene.hpp"
#include "game/screens/MainMenuScreen.hpp"
#include "core/Config.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <memory>
#include <stdexcept>
#include <algorithm>

LoadGameScreen::LoadGameScreen(sf::RenderWindow &window,
                               SceneManager &scenes,
                               float internalW,
                               float internalH,
                               int tileSize)
    : m_window(window),
      m_scenes(scenes)
{
    m_uiView.setSize({internalW, internalH});
    m_uiView.setCenter({internalW * 0.5f, internalH * 0.5f});

    loadFont();
    initTextures(internalW, internalH);
    listSaves(internalW, internalH);
    setButtons(internalW, internalH);
    setBackGround();

    m_backBtn.setSize({60.f, 18.f});
    m_backBtn.setFillColor(sf::Color(30, 30, 30, 220));
    m_backBtn.setOutlineColor(sf::Color(120, 80, 85));
    m_backBtn.setOutlineThickness(1.f);
    m_backBtn.setPosition({8.f, 8.f});

    m_backText.emplace(m_font, "Back", 10);
    m_backText->setFillColor(sf::Color(245, 240, 220));

    const sf::FloatRect bb = m_backText->getLocalBounds();
    m_backText->setOrigin({bb.position.x + bb.size.x * 0.5f,
                           bb.position.y + bb.size.y * 0.5f});
    m_backText->setPosition({m_backBtn.getPosition().x + m_backBtn.getSize().x * 0.5f,
                             m_backBtn.getPosition().y + m_backBtn.getSize().y * 0.5f});
}

void LoadGameScreen::loadFont()
{
    if (!m_font.openFromFile(cfg::FontPixel))
    {
        throw std::runtime_error("LoadGameScreen: failed to load font");
    }
    m_font.setSmooth(false);
}

void LoadGameScreen::initTextures(float internalW, float internalH)
{
    m_bgTexture.emplace();
    if (!m_bgTexture->loadFromFile("assets/pixelcity.png"))
    {
        throw std::runtime_error("LoadGameScreen: failed to load assets/pixelcity.png");
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

void LoadGameScreen::listSaves(float internalW, float internalH)
{
    m_saves = SaveSystem::listSaves();
    if (m_saves.empty())
    {
        m_emptyLabel.emplace(m_font, "No saved games", 12);
        m_emptyLabel->setFillColor(sf::Color(245, 240, 220));
        const sf::FloatRect b = m_emptyLabel->getLocalBounds();
        m_emptyLabel->setOrigin({b.position.x + b.size.x * 0.5f, b.position.y + b.size.y * 0.5f});
        m_emptyLabel->setPosition({internalW * 0.5f, internalH * 0.5f});
        return;
    }
}

void LoadGameScreen::setButtons(float internalW, float internalH)
{
    m_deleteButtons.clear();
    m_deleteTexts.clear();
    m_items.clear();
    m_bounds.clear();
    m_buttons.clear();
    const sf::Vector2f buttonSize{180.f, 24.f};
    const float startY = internalH * 0.5f - 32.f;
    const float centerX = internalW * 0.5f;
    constexpr size_t MaxSaves = 5;
    const size_t count = std::min(m_saves.size(), MaxSaves);
    for (size_t i = 0; i < m_saves.size(); ++i)
    {
        sf::RectangleShape btn(buttonSize);
        btn.setPosition({centerX - buttonSize.x * 0.5f, startY + i * 30.f});
        btn.setFillColor(sf::Color(30, 30, 30, 220));
        btn.setOutlineColor(sf::Color(120, 80, 85));
        btn.setOutlineThickness(1.f);
        m_buttons.push_back(btn);

        sf::Text t(m_font, m_saves[i].displayName, 12);
        t.setFillColor(sf::Color(245, 240, 220));
        const sf::FloatRect b = t.getLocalBounds();
        t.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y + b.size.y * 0.5f});
        const sf::Vector2f btnPos = m_buttons.back().getPosition();
        t.setPosition({btnPos.x + buttonSize.x * 0.5f, btnPos.y + buttonSize.y * 0.5f - 1.f});
        m_items.push_back(t);
        m_bounds.push_back(t.getGlobalBounds());

        sf::RectangleShape del({18.f, 18.f});
        del.setFillColor(sf::Color(120, 50, 50, 220));
        del.setOutlineColor(sf::Color(180, 90, 90));
        del.setOutlineThickness(1.f);

        del.setPosition({btn.getPosition().x + buttonSize.x + 6.f,
                         btn.getPosition().y + 3.f});

        m_deleteButtons.push_back(del);

        sf::Text x(m_font, "X", 10);
        x.setFillColor(sf::Color::White);

        const sf::FloatRect xb = x.getLocalBounds();
        x.setOrigin({xb.position.x + xb.size.x * 0.5f,
                     xb.position.y + xb.size.y * 0.5f});
        x.setPosition({del.getPosition().x + 9.f,
                       del.getPosition().y + 9.f});

        m_deleteTexts.push_back(x);
    }
}

void LoadGameScreen::setBackGround()
{
    if (m_buttons.empty())
        return;
    float left = m_buttons[0].getPosition().x;
    float top = m_buttons[0].getPosition().y;
    float right = left + m_buttons[0].getSize().x;
    float bottom = top + m_buttons[0].getSize().y;
    for (size_t i = 1; i < m_buttons.size(); ++i)
    {
        const sf::Vector2f pos = m_buttons[i].getPosition();
        const sf::Vector2f size = m_buttons[i].getSize();
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

void LoadGameScreen::handleEvent(const sf::Event &e)
{

    if (const auto *key = e.getIf<sf::Event::KeyPressed>())
    {
        if (key->code == sf::Keyboard::Key::Escape)
        {
            m_scenes.set(std::make_unique<MainMenuScreen>(
                m_window,
                m_scenes,
                static_cast<float>(cfg::InternalW),
                static_cast<float>(cfg::InternalH),
                cfg::TileSize));
        }
        return;
    }

    if (!e.is<sf::Event::MouseButtonPressed>())
        return;

    const sf::Vector2f mouse =
        m_window.mapPixelToCoords(
            sf::Mouse::getPosition(m_window), m_uiView);

    if (m_backBtn.getGlobalBounds().contains(mouse))
    {
        m_scenes.set(std::make_unique<MainMenuScreen>(
            m_window,
            m_scenes,
            static_cast<float>(cfg::InternalW),
            static_cast<float>(cfg::InternalH),
            cfg::TileSize));
        return;
    }

    for (size_t i = 0; i < m_deleteButtons.size(); ++i)
    {
        if (!m_deleteButtons[i].getGlobalBounds().contains(mouse))
            continue;

        SaveSystem::deleteSave(m_saves[i].fileName);

        listSaves(cfg::InternalW, cfg::InternalH);
        setButtons(cfg::InternalW, cfg::InternalH);
        setBackGround();
        return;
    }

    for (size_t i = 0; i < m_bounds.size(); ++i)
    {
        if (!m_bounds[i].contains(mouse))
            continue;

        auto state = SaveSystem::loadState(m_saves[i].fileName);
        if (!state)
            return;

        m_scenes.set(std::make_unique<BuilderScene>(
            m_window,
            m_scenes,
            static_cast<float>(cfg::InternalW),
            static_cast<float>(cfg::InternalH),
            cfg::TileSize,
            state->cityName,
            state));
        return;
    }
}

void LoadGameScreen::update(float)
{
}

void LoadGameScreen::render(sf::RenderTarget &target)
{
    target.setView(m_uiView);
    if (m_bgSprite)
        target.draw(*m_bgSprite);
    target.draw(m_backBtn);
    if (m_backText)
        target.draw(*m_backText);
    if (m_emptyLabel)
    {
        target.draw(*m_emptyLabel);
        return;
    }
    target.draw(m_menuBg);
    for (auto &b : m_buttons)
        target.draw(b);
    for (auto &t : m_items)
        target.draw(t);

    for (auto &d : m_deleteButtons)
        target.draw(d);
    for (auto &t : m_deleteTexts)
        target.draw(t);
}
