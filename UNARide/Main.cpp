#include <SFML/Graphics.hpp>
#include "Map.h"
#include "RouteManager.h"
#include "CarController.h"
#include "UIManager.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "UNARide");

    sf::Texture mapTexture, carTexture;
    if (!mapTexture.loadFromFile("../map.png") || !carTexture.loadFromFile("../car.png")) {
        std::cerr << "Error cargando texturas." << std::endl;
        return 1;
    }

    sf::Sprite mapSprite(mapTexture);
    mapSprite.setScale(
        static_cast<float>(window.getSize().x) / mapTexture.getSize().x,
        static_cast<float>(window.getSize().y) / mapTexture.getSize().y
    );
    sf::Sprite carSprite(carTexture);
    carSprite.setScale(0.2f, 0.2f);

    sf::Font font;
    if (!font.loadFromFile("../arial.ttf")) {
        std::cerr << "Error cargando la fuente." << std::endl;
        return 1;
    }

    Map map;
    map.loadNodesAndStreets("Nodes.txt");

    UIManager uiManager(window, font);
    RouteManager routeManager(map);
    CarController carController(carSprite, 75.0f);

    bool useDijkstra = true;
    bool startMovement = false;
    bool routeCalculated = false;
    bool algorithmSelected = false;
    bool carVisible = false;  
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

                if (uiManager.startButton.getGlobalBounds().contains(mousePos) && routeCalculated && algorithmSelected && routeManager.isStartNodeSelected() && routeManager.isEndNodeSelected()) {
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
                else if (uiManager.floydCheckBox.getGlobalBounds().contains(mousePos)) {
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
