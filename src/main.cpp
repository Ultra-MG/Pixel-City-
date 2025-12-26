#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>

int main()
{
    std::cout<<"hello";
    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u{800u, 600u}),
        "Pixel City"
    );

    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);
        window.display();
    }

    return 0;
}
