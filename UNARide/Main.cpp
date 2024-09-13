#include <SFML/Graphics.hpp>
#include "Map.h"
#include "iostream"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "UNARide");

    sf::Texture mapTexture;
    if (!mapTexture.loadFromFile("../map.png")) {
        std::cerr << "Failed to load the map." << std::endl;
        return 1;
    }

    sf::Sprite mapSprite(mapTexture);
    mapSprite.setScale(
        static_cast<float>(window.getSize().x) / mapTexture.getSize().x,
        static_cast<float>(window.getSize().y) / mapTexture.getSize().y
    );

    sf::Texture carTexture;
    if (!carTexture.loadFromFile("../car.png")) {
        std::cerr << "Failed to load the car image." << std::endl;
        return 1;
    }

    sf::Sprite carSprite(carTexture);
    carSprite.setScale(0.1f, 0.1f);

    Map map;
    map.loadNodesAndStreets("Nodes.txt");

    float speed = 75.0f;
    sf::Clock clock;
    std::size_t startNode = 9, endNode = 39;
    std::vector<std::size_t> path = map.dijkstra(startNode, endNode);
    carSprite.setPosition(map.getNodes()[startNode].getPosition());

    std::size_t currentNodeInPath = 0;
    sf::Vector2f destination = map.getNodes()[path[currentNodeInPath + 1]].getPosition();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        float deltaTime = clock.restart().asSeconds();

        if (currentNodeInPath < path.size() - 1) {
            std::size_t currentNode = path[currentNodeInPath];
            std::size_t nextNode = path[currentNodeInPath + 1];

            for (const auto& street : map.getStreets()) {
                if ((street.getNode1() == currentNode && street.getNode2() == nextNode) ||
                    (street.isBidirectional() && street.getNode1() == nextNode && street.getNode2() == currentNode)) {
                    sf::Vector2f currentPosition = carSprite.getPosition();
                    sf::Vector2f destination = (street.getNode1() == currentNode) ? street.getEndPosition() : street.getStartPosition();
                    sf::Vector2f direction = destination - currentPosition;
                    float distance = std::hypot(direction.x, direction.y);

                    if (distance > 1.0f) {
                        sf::Vector2f movement = direction / distance * speed * deltaTime;
                        carSprite.move(movement);
                    }
                    else {
                        currentNodeInPath++;
                        if (currentNodeInPath < path.size()) {
                            carSprite.setPosition(map.getNodes()[path[currentNodeInPath]].getPosition());
                        }
                    }
                    break;
                }
            }
        }

        window.clear();
        window.draw(mapSprite);
        map.draw(window);
        window.draw(carSprite);
        window.display();
    }

    return 0;
}
