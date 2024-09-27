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
    carSprite.setOrigin(static_cast<float>(carTextureUp.getSize().x) / 2.0f, static_cast<float>(carTextureUp.getSize().y) / 2.0f);

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
    bool showWeights = false; 
    bool showStreets = true;
    sf::Clock gameClock;

    std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>> floydWarshallResult;
    bool selectingTrafficNodes = false;
    std::size_t trafficStartNode = -1;
    std::size_t trafficEndNode = -1;
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

                // Dentro del bucle principal de eventos de mousePressed
                if (uiManager.trafficComboBox.getGlobalBounds().contains(mousePos)) {
                    uiManager.selectedTrafficIndex = (uiManager.selectedTrafficIndex + 1) % uiManager.trafficOptions.size();
                    uiManager.selectedTrafficText.setString(uiManager.trafficOptions[uiManager.selectedTrafficIndex]);
                    selectingTrafficNodes = true;
                    trafficStartNode = -1;
                    trafficEndNode = -1;

                    // Permitir modificar el peso de la calle incluso si hay una ruta marcada
                    if (routeCalculated) {
                        std::cout << "Permitiendo modificar el peso de las calles tras seleccionar un tipo de tráfico." << std::endl;
                    }
                }

                // Lógica para seleccionar nodos para cambiar el tráfico
                if (selectingTrafficNodes) {
                    std::size_t clickedNode = routeManager.findClosestNode(mousePos);
                    if (clickedNode != std::size_t(-1)) {
                        if (trafficStartNode == -1) {
                            trafficStartNode = clickedNode;
                            std::cout << "Nodo de inicio para el tráfico seleccionado: " << trafficStartNode << std::endl;
                        }
                        else if (trafficEndNode == -1 && clickedNode != trafficStartNode) {
                            trafficEndNode = clickedNode;
                            std::cout << "Nodo final para el tráfico seleccionado: " << trafficEndNode << std::endl;

                            for (auto& street : map.getStreets()) {
                                if ((street.getNode1() == trafficStartNode && street.getNode2() == trafficEndNode) ||
                                    (street.getNode1() == trafficEndNode && street.getNode2() == trafficStartNode)) {

                                    int trafficMultiplier = uiManager.getTrafficMultiplier();
                                    float newWeight = street.getWeight() * trafficMultiplier;
                                    street.setWeight(newWeight);

                                    std::cout << "Nuevo peso de la calle: " << newWeight << std::endl;

                                    // Recalcular la ruta con el nuevo peso
                                    routeManager.calculateRoute(useDijkstra, floydWarshallResult);
                                    float totalWeight = routeManager.calculateTotalWeight();
                                    float totalCost = routeManager.calculateTotalCost();
                                    uiManager.setTotalWeight(totalWeight);
                                    uiManager.setTotalCost(totalCost);
                                    routeCalculated = true;

                                    // Resetea la selección para permitir otros ajustes de tráfico
                                    selectingTrafficNodes = false;
                                    break;
                                }
                            }
                        }
                    }
                }

                if (uiManager.clearButton.getGlobalBounds().contains(mousePos)) {
                    std::cout << "Botón 'Limpiar' presionado." << std::endl;
                    routeManager.resetRoute();
                    startMovement = false;
                    routeCalculated = false;
                    carVisible = false;
                    showWeights = false;
                    algorithmSelected = false;
                    uiManager.resetAlgorithmSelected();
                    uiManager.setTotalWeight(0.0f);
                    uiManager.setTotalCost(0.0f);
                }

                if (uiManager.clearButton.getGlobalBounds().contains(mousePos)) {
                    std::cout << "Botón 'Limpiar' presionado." << std::endl;
                    routeManager.resetRoute();
                    startMovement = false;
                    routeCalculated = false;
                    carVisible = false;
                    showWeights = false;
                    algorithmSelected = false;
                    uiManager.resetAlgorithmSelected(); 
                    uiManager.setTotalWeight(0.0f);
                    uiManager.setTotalCost(0.0f);
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
                    routeManager.resetRoute(); 
                }
                if (uiManager.floydCheckBox.getGlobalBounds().contains(mousePos)) {
                    useDijkstra = false;
                    algorithmSelected = true;  
                    floydWarshallResult = map.floydWarshall();
                    uiManager.setAlgorithmSelected(false); 
                    std::cout << "Algoritmo Floyd-Warshall seleccionado." << std::endl;
                    routeManager.calculateRoute(useDijkstra, floydWarshallResult);
                    routeCalculated = true;
                    routeManager.resetRoute(); 
                }

                if (uiManager.toggleWeightsButton.getGlobalBounds().contains(mousePos)) {
                    showWeights = !showWeights; 
                    std::cout << (showWeights ? "Mostrando pesos." : "Ocultando pesos.") << std::endl;
                }

                if (uiManager.toggleStreetsButton.getGlobalBounds().contains(mousePos)) {
                    showStreets = !showStreets;  
                    std::cout << (showStreets ? "Mostrando calles." : "Ocultando calles.") << std::endl;
                }

                routeManager.selectNode(mousePos);
                if (routeManager.isStartNodeSelected() && routeManager.isEndNodeSelected() && algorithmSelected) {
                    std::cout << "Calculando la ruta con tráfico..." << std::endl;

                    size_t startNode = routeManager.getStartNode();
                    size_t endNode = routeManager.getEndNode();

                    for (auto& street : map.getStreets()) { 
                        if ((street.getNode1() == startNode && street.getNode2() == endNode) ||
                            (street.getNode1() == endNode && street.getNode2() == startNode)) {

                            int trafficMultiplier = uiManager.getTrafficMultiplier();
                            float newWeight = street.getWeight() * trafficMultiplier;
                            street.setWeight(newWeight); 

                            std::cout << "Tráfico aplicado: " << uiManager.trafficOptions[uiManager.selectedTrafficIndex]
                                << " al tramo de la calle (" << startNode << " a " << endNode
                                << ") con nuevo peso: " << newWeight << std::endl;
                        }
                    }
                    routeManager.calculateRoute(useDijkstra, floydWarshallResult);
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

        if (showStreets) {
            map.draw(window);  
        }

        if (showWeights) {
            map.drawWeights(window, font);  
        }

        if (routeCalculated) {
            routeManager.drawRoute(window);
            float totalWeight = routeManager.calculateTotalWeight();
            float totalCost = routeManager.calculateTotalCost();
            uiManager.setTotalWeight(totalWeight);
            uiManager.setTotalCost(totalCost);
        }
        if (carVisible) {
            window.draw(carSprite);
        }
        uiManager.drawUI(window);
        window.display();
    }

    return 0;
}
