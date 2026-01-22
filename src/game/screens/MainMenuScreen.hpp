#pragma once

#include "engine/Scene.hpp"
#include "engine/SceneManager.hpp"
#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>

class MainMenuScreen : public Scene
{
public:
    MainMenuScreen(sf::RenderWindow &window,
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

    std::optional<sf::Texture> m_bgTexture;
    std::optional<sf::Sprite> m_bgSprite;

    std::vector<sf::Text> m_menuItems;
    std::vector<sf::FloatRect> m_menuBounds;
    std::vector<sf::RectangleShape> m_menuButtons;
    sf::RectangleShape m_menuBg;
};
