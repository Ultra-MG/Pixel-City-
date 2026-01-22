#pragma once

#include "engine/Scene.hpp"
#include "engine/SceneManager.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>

class SplashScreen : public Scene
{
public:
    SplashScreen(sf::RenderWindow &window,
                SceneManager &scenes,
                float internalW,
                float internalH,
                int tileSize);

    void handleEvent(const sf::Event &e) override;
    void update(float dt) override;
    void render(sf::RenderTarget &target) override;

private:
    sf::RenderWindow &m_window;
    SceneManager &m_scenes;

    sf::View m_uiView;
    sf::Font m_font;
    std::optional<sf::Text> m_title;
    std::optional<sf::Text> m_hint;
    sf::RectangleShape m_textBg;
    bool m_showHint = true;
    float m_blinkTimer = 0.f;

    std::optional<sf::Texture> m_bgTexture;
    std::optional<sf::Sprite> m_bgSprite;
};
