#include <SFML/Graphics.hpp>
#include "Map.h"
#include "RouteManager.h"
#include "CarController.h"
#include "UIManager.h"
#include <iostream>
#include <limits>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "UNARide", sf::Style::Titlebar | sf::Style::Close);

    sf::Texture mapTexture;
    if (!mapTexture.loadFromFile("../map.png")) {
        std::cerr << "Error loading map texture." << std::endl;
        return 1;
    }

    std::size_t node1 = std::numeric_limits<std::size_t>::max();
    std::size_t node2 = std::numeric_limits<std::size_t>::max();
    bool awaitingNodeSelection = false;
    int nodesSelected = 0;

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
    CarController carController(carSprite, 100.f, carTextureUp, carTextureDown, carTextureLeft, carTextureRight, uiManager, routeManager);

    bool useDijkstra = true, startMovement = false, routeCalculated = false, algorithmSelected = false, carVisible = false;
    bool showWeights = false;
    bool showStreets = true;
    bool applyingTrafficChanges = false;
    bool isChangingRoute = false;
    sf::Clock gameClock;

    std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>> floydWarshallResult;
    std::size_t trafficStartNode = std::size_t(-1);
    std::size_t trafficEndNode = std::size_t(-1);
    bool selectingTrafficNodes = false;

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

                if (uiManager.trafficComboBox.getGlobalBounds().contains(mousePos)) {
                    uiManager.selectedTrafficIndex = (uiManager.selectedTrafficIndex + 1) % uiManager.trafficOptions.size();
                    uiManager.selectedTrafficText.setString(uiManager.trafficOptions[uiManager.selectedTrafficIndex]);

                    std::cout << "Tráfico seleccionado: " << uiManager.trafficOptions[uiManager.selectedTrafficIndex] << std::endl;
                    selectingTrafficNodes = true;
                    trafficStartNode = std::size_t(-1);
                    trafficEndNode = std::size_t(-1);
                    carController.stopAtNextNode();
                    applyingTrafficChanges = true;
                }

                if (applyingTrafficChanges && carController.isStopped()) {
                    if (routeManager.isStartNodeSelected() && carController.hasValidRoute()) {
                        std::size_t currentCarNode = carController.getCurrentNode(map);
                        std::size_t newDestination = routeManager.getEndNode();

                        if (trafficStartNode != std::size_t(-1) && trafficEndNode != std::size_t(-1)) {
                            Street* selectedStreet = const_cast<Street*>(map.getStreetBetweenNodes(trafficStartNode, trafficEndNode));
                            if (selectedStreet) {
                                int trafficMultiplier = uiManager.getTrafficMultiplier();
                                float newWeight = selectedStreet->getWeight() * trafficMultiplier;
                                selectedStreet->setWeight(newWeight);

                                std::cout << "Nuevo peso de la calle entre los nodos " << trafficStartNode
                                    << " y " << trafficEndNode << ": " << newWeight << std::endl;

                                routeManager.calculateNewRoute(newDestination, currentCarNode, useDijkstra, floydWarshallResult);
                                float totalWeight = routeManager.calculateTotalWeight(currentCarNode);  
                                float totalCost = routeManager.calculateTotalCost(); 
                                uiManager.setTotalWeight(totalWeight);  
                                uiManager.setTotalCost(totalCost);
                                carController.startMovement(routeManager.getNewPath(), map, true);
                                applyingTrafficChanges = false;
                            }
                        }
                    }
                    else {
                        std::cerr << "Error: no se ha seleccionado una ruta válida." << std::endl;
                    }
                }

                if (selectingTrafficNodes) {
                    std::size_t selectedNode = routeManager.findClosestNode(mousePos);
                    if (selectedNode != std::size_t(-1)) {
                        if (trafficStartNode == std::size_t(-1)) {
                            trafficStartNode = selectedNode;
                            std::cout << "Nodo de inicio para el tráfico seleccionado: " << trafficStartNode << std::endl;
                        }
                        else if (trafficEndNode == std::size_t(-1) && selectedNode != trafficStartNode) {
                            trafficEndNode = selectedNode;
                            std::cout << "Nodo final para el tráfico seleccionado: " << trafficEndNode << std::endl;

                            Street* selectedStreet = const_cast<Street*>(map.getStreetBetweenNodes(trafficStartNode, trafficEndNode));
                            if (selectedStreet) {
                                int trafficMultiplier = uiManager.getTrafficMultiplier();
                                float newWeight = selectedStreet->getWeight() * trafficMultiplier;
                                selectedStreet->setWeight(newWeight);

                                std::cout << "Nuevo peso de la calle entre los nodos " << trafficStartNode
                                    << " y " << trafficEndNode << ": " << newWeight << std::endl;

                                routeManager.calculateRoute(useDijkstra, floydWarshallResult);
                                routeCalculated = true;
                                selectingTrafficNodes = false;
                            }
                            else {
                                std::cout << "No se encontró una calle entre los nodos seleccionados." << std::endl;
                            }
                        }
                    }
                }

                if (uiManager.changeRouteButton.getGlobalBounds().contains(mousePos)) {
                    std::cout << "Botón 'Cambiar ruta' presionado. El carro se detendrá en el siguiente nodo más cercano." << std::endl;
                    carController.stopAtNextNode();
                    isChangingRoute = true;
                }

                if (awaitingNodeSelection) {
                    std::size_t selectedNode = routeManager.findClosestNode(mousePos);
                    if (selectedNode != std::numeric_limits<std::size_t>::max()) {
                        if (nodesSelected == 0) {
                            node1 = selectedNode;
                            nodesSelected++;
                        }
                        else if (nodesSelected == 1 && routeManager.areNodesConnected(node1, selectedNode)) {
                            node2 = selectedNode;
                            awaitingNodeSelection = false;

                            Street* selectedStreet = const_cast<Street*>(map.getStreetBetweenNodes(node1, node2));
                            if (selectedStreet) {
                                int trafficMultiplier = uiManager.getTrafficMultiplier();
                                float newWeight = selectedStreet->getWeight() * trafficMultiplier;
                                selectedStreet->setWeight(newWeight);

                                std::cout << "Peso actualizado para la calle entre los nodos " << node1
                                    << " y " << node2 << ": " << newWeight << std::endl;
                            }
                            else {
                                std::cout << "No se encontró una calle entre los nodos seleccionados." << std::endl;
                            }
                        }
                    }
                }

                if (isChangingRoute) {
                    std::size_t newDestination = routeManager.findClosestNode(mousePos);
                    if (newDestination != std::size_t(-1)) {
                        std::cout << "Nuevo destino seleccionado: " << newDestination << std::endl;

                        if (routeManager.isStartNodeSelected() && carController.hasValidRoute()) {
                            std::size_t currentCarNode = carController.getCurrentNode(map);

                            bool useDijkstra = uiManager.isDijkstraSelected();
                            floydWarshallResult = map.floydWarshall();

                            routeManager.setTotalWeight(0.0f);
                            uiManager.setTotalWeight(0.0f);
                            uiManager.setTotalCost(0.0f);
                            routeManager.calculateNewRoute(newDestination, currentCarNode, useDijkstra, floydWarshallResult);

                            float totalWeight = routeManager.calculateTotalWeight(currentCarNode);
                            float totalCost = routeManager.calculateTotalCost();

                            uiManager.setTotalWeight(totalWeight);
                            uiManager.setTotalCost(totalCost);

                            carController.startMovement(routeManager.getNewPath(), map, true);
                            isChangingRoute = false;
                        }
                        else {
                            std::cerr << "Error: no se ha seleccionado una ruta válida." << std::endl;
                        }
                    }
                }

                if (uiManager.startButton.getGlobalBounds().contains(mousePos) && routeCalculated && algorithmSelected &&
                    routeManager.isStartNodeSelected() && routeManager.isEndNodeSelected()) {

                    std::cout << "Botón 'Iniciar' presionado." << std::endl;
                    carController.startMovement(routeManager.getPath(), map, false);
                    startMovement = true;
                    carVisible = true;
                }

                if (uiManager.clearButton.getGlobalBounds().contains(mousePos)) {
                    std::cout << "Botón 'Limpiar' presionado." << std::endl;
                    routeManager.resetRoute();
                    startMovement = false;
                    routeCalculated = false;
                    carVisible = false; 
                    showWeights = false;
                    algorithmSelected = false;
                    isChangingRoute = false;
                    showStreets = false;
                    uiManager.selectedTrafficIndex = 0;
                    uiManager.selectedTrafficText.setString(uiManager.trafficOptions[uiManager.selectedTrafficIndex]);

                    uiManager.resetAlgorithmSelected();
                    uiManager.setTotalWeight(0.0f);
                    uiManager.setTotalCost(0.0f);
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
                    std::cout << "Calculando la ruta..." << std::endl;

                    size_t startNode = routeManager.getStartNode();
                    size_t endNode = routeManager.getEndNode();

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
            routeManager.drawNewRoute(window);
        }

        uiManager.drawUI(window);

        if (carVisible) {
            window.draw(carSprite);
        }

        window.display();
    }

    return 0;
}
