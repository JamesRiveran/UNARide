#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "UNARide");

    sf::Texture texture;
    if (!texture.loadFromFile("../map.png"))
    {
        std::cerr << "Failed to load image." << std::endl;
        return -1;
    }

    sf::Sprite sprite;
    sprite.setTexture(texture);

    sf::Vector2u textureSize = texture.getSize(); 
    sf::Vector2u windowSize = window.getSize();

    sprite.setScale(
        static_cast<float>(windowSize.x) / textureSize.x,
        static_cast<float>(windowSize.y) / textureSize.y
    );

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
