#pragma once

#include "engine/Scene.hpp"
#include "engine/SceneManager.hpp"
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

class NameEntryScreen : public Scene
{
public:
    NameEntryScreen(sf::RenderWindow &window,
                    SceneManager &scenes,
                    float internalW,
                    float internalH,
                    int tileSize);

    void handleEvent(const sf::Event &e) override;
    void update(float dt) override;
    void render(sf::RenderTarget &target) override;

private:
    void refreshText();

    sf::RenderWindow &m_window;
    SceneManager &m_scenes;
    sf::View m_uiView;

    sf::Font m_font;
    std::optional<sf::Texture> m_bgTexture;
    std::optional<sf::Sprite> m_bgSprite;

    std::optional<sf::Text> m_title;
    std::optional<sf::Text> m_input;
    sf::RectangleShape m_inputBox;
    std::string m_name;
    float m_cursorTimer = 0.f;
    bool m_showCursor = true;
};
