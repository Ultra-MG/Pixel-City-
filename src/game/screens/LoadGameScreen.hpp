#pragma once

#include "engine/Scene.hpp"
#include "engine/SceneManager.hpp"
#include "game/SaveSystem.hpp"
#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>

class LoadGameScreen : public Scene
{
public:
    LoadGameScreen(sf::RenderWindow &window,
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
    sf::RectangleShape m_backBtn;
    std::optional<sf::Text> m_backText;

    sf::Font m_font;
    std::optional<sf::Texture> m_bgTexture;
    std::optional<sf::Sprite> m_bgSprite;
    
    std::vector<sf::RectangleShape> m_deleteButtons;
    std::vector<sf::Text> m_deleteTexts;

    std::vector<SaveInfo> m_saves;
    std::vector<sf::Text> m_items;
    std::vector<sf::FloatRect> m_bounds;
    std::vector<sf::RectangleShape> m_buttons;
    sf::RectangleShape m_menuBg;
    std::optional<sf::Text> m_emptyLabel;

    void loadFont();
    void initTextures(float internalW, float internalH);
    void listSaves(float internalW, float internalH);
    void setButtons(float internalW,
                    float internalH);
    void setBackGround();
};
