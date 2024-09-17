#include <SFML/Graphics.hpp>
#include "Map.h"
#include "RouteManager.h"
#include "CarController.h"
#include "UIManager.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "UNARide");

    sf::Texture mapTexture;
    if (!mapTexture.loadFromFile("../map.png")) {
        std::cerr << "Error loading map texture." << std::endl;
        return 1;
    }

    sf::Sprite mapSprite(mapTexture);
    mapSprite.setScale(
        static_cast<float>(window.getSize().x) / mapTexture.getSize().x,
        static_cast<float>(window.getSize().y) / mapTexture.getSize().y
    );

    sf::Texture carTextureUp, carTextureDown, carTextureLeft, carTextureRight;
    if (!carTextureUp.loadFromFile("../car_up.png") ||
        !carTextureDown.loadFromFile("../car_down.png") ||
        !carTextureLeft.loadFromFile("../car_left.png") ||
        !carTextureRight.loadFromFile("../car_right.png")) {
        std::cerr << "Error loading car textures." << std::endl;
        return 1;
    }

    sf::Sprite carSprite(carTextureUp);
    carSprite.setScale(0.15f, 0.15f);
    carSprite.setOrigin(carTextureUp.getSize().x / 2, carTextureUp.getSize().y / 2);

    sf::Font font;
    if (!font.loadFromFile("../arial.ttf")) {
        std::cerr << "Error loading font." << std::endl;
        return 1;
    }

    Map map;
    map.loadNodesAndStreets("Nodes.txt");

    UIManager uiManager(window, font);
    RouteManager routeManager(map);
    CarController carController(carSprite, 75.0f, carTextureUp, carTextureDown, carTextureLeft, carTextureRight);

    bool useDijkstra = true, startMovement = false, routeCalculated = false, algorithmSelected = false, carVisible = false;
    sf::Clock gameClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::Resized) {
                uiManager.resizeUI(window);
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (uiManager.clearButton.getGlobalBounds().contains(mousePos)) {
                    std::cout << "Botón 'Limpiar' presionado." << std::endl;
                    routeManager.resetRoute();
                    startMovement = false;
                    routeCalculated = false;
                    carVisible = false;
                }

                if (uiManager.startButton.getGlobalBounds().contains(mousePos) && routeCalculated && algorithmSelected &&
                    routeManager.isStartNodeSelected() && routeManager.isEndNodeSelected()) {
                    std::cout << "Botón 'Iniciar' presionado." << std::endl;
                    carController.startMovement(routeManager.getPath(), map);
                    startMovement = true;
                    carVisible = true;
                }

                if (uiManager.dijkstraCheckBox.getGlobalBounds().contains(mousePos)) {
                    useDijkstra = true;
                    algorithmSelected = true;
                    uiManager.setAlgorithmSelected(true);  
                    std::cout << "Algoritmo Dijkstra seleccionado." << std::endl;
                }
                if (uiManager.floydCheckBox.getGlobalBounds().contains(mousePos)) {
                    useDijkstra = false;
                    algorithmSelected = true;
                    uiManager.setAlgorithmSelected(false); 
                    std::cout << "Algoritmo Floyd-Warshall seleccionado." << std::endl;
                }

                routeManager.selectNode(mousePos);
                if (routeManager.isStartNodeSelected() && routeManager.isEndNodeSelected() && algorithmSelected) {
                    std::cout << "Calculando la ruta..." << std::endl;
                    routeManager.calculateRoute(useDijkstra);
                    routeCalculated = true;
                }
            }
        }

        float deltaTime = gameClock.restart().asSeconds();
        if (startMovement) {
            carController.update(deltaTime, map);
        }

        window.clear();
        window.draw(mapSprite);
        map.draw(window);
        if (routeCalculated) {
            routeManager.drawRoute(window);
        }
        if (carVisible) {
            window.draw(carSprite);
        }
        uiManager.drawUI(window);
        window.display();
    }

    return 0;
}
