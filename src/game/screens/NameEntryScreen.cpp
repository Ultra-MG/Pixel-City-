#include "game/screens/NameEntryScreen.hpp"
#include "game/BuilderScene.hpp"
#include "game/screens/MainMenuScreen.hpp"
#include "core/Config.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <memory>
#include <stdexcept>

NameEntryScreen::NameEntryScreen(sf::RenderWindow &window,
                                 SceneManager &scenes,
                                 float internalW,
                                 float internalH,
                                 int tileSize)
    : m_window(window),
      m_scenes(scenes)
{
    m_uiView.setSize({internalW, internalH});
    m_uiView.setCenter({internalW * 0.5f, internalH * 0.5f});

    if (!m_font.openFromFile("assets/fonts/pixelFont.ttf"))
    {
        throw std::runtime_error("NameEntryScreen: failed to load font");
    }
    m_font.setSmooth(false);

    m_title.emplace(m_font, "Name Your City", 14);
    m_title->setFillColor(sf::Color(245, 240, 220));
    const sf::FloatRect titleBounds = m_title->getLocalBounds();
    m_title->setOrigin({titleBounds.position.x + titleBounds.size.x * 0.5f,
                        titleBounds.position.y + titleBounds.size.y * 0.5f});
    m_title->setPosition({internalW * 0.5f, internalH * 0.38f});

    m_input.emplace(m_font, "", 12);
    m_input->setFillColor(sf::Color::White);

    m_inputBox.setSize({180.f, 26.f});
    m_inputBox.setFillColor(sf::Color(30, 30, 30, 220));
    m_inputBox.setOutlineColor(sf::Color(120, 80, 85));
    m_inputBox.setOutlineThickness(1.f);
    m_inputBox.setPosition({internalW * 0.5f - 90.f, internalH * 0.48f});

    m_bgTexture.emplace();
    if (!m_bgTexture->loadFromFile("assets/pixelcity.png"))
    {
        throw std::runtime_error("NameEntryScreen: failed to load assets/pixelcity.png");
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

    refreshText();
}

void NameEntryScreen::handleEvent(const sf::Event &e)
{
    if (const auto *text = e.getIf<sf::Event::TextEntered>())
    {
        const std::uint32_t code = text->unicode;
        if (code == 8)
        {
            if (!m_name.empty())
                m_name.pop_back();
        }
        else if (code >= 32 && code <= 126)
        {
            if (m_name.size() < 18)
                m_name.push_back(static_cast<char>(code));
        }
        refreshText();
        return;
    }

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
            return;
        }
        if (key->code == sf::Keyboard::Key::Enter)
        {
            const std::string name = m_name.empty() ? "New City" : m_name;
            m_scenes.set(std::make_unique<BuilderScene>(
                m_window,
                static_cast<float>(cfg::InternalW),
                static_cast<float>(cfg::InternalH),
                cfg::TileSize,
                name));
            return;
        }
    }
}

void NameEntryScreen::update(float dt)
{
    m_cursorTimer += dt;
    if (m_cursorTimer >= 0.5f)
    {
        m_cursorTimer = 0.f;
        m_showCursor = !m_showCursor;
        refreshText();
    }
}

void NameEntryScreen::render(sf::RenderTarget &target)
{
    target.setView(m_uiView);
    if (m_bgSprite)
        target.draw(*m_bgSprite);
    if (m_title)
        target.draw(*m_title);
    target.draw(m_inputBox);
    if (m_input)
        target.draw(*m_input);
}

void NameEntryScreen::refreshText()
{
    std::string display = m_name;
    if (m_showCursor)
        display += "_";
    if (!m_input)
        return;
    m_input->setString(display);

    const sf::FloatRect inputBounds = m_input->getLocalBounds();
    const sf::Vector2f boxPos = m_inputBox.getPosition();
    const sf::Vector2f boxSize = m_inputBox.getSize();
    m_input->setPosition({boxPos.x + 6.f, boxPos.y + (boxSize.y - inputBounds.size.y) * 0.5f - inputBounds.position.y});
}
