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
    sf::Clock travelClock;
    bool isTimerRunning = false;
    float timeCostPerSecond = 2.0f;
    float totalTimeCost = 0.0f;
    float totalCompleteCost = 0.0f;

    std::size_t node1 = std::numeric_limits<std::size_t>::max();
    std::size_t node2 = std::numeric_limits<std::size_t>::max();
    bool awaitingNodeSelection = false;
    bool awaitingStreetClose = false;
    bool awaitingStreetOpen = false;
    bool isSelectingNewTrip = false;  
    bool selectingNewDestination = false;  
    bool carVisible = false;  
    bool readyToStartNewTrip = false;
    bool newTrip = false;
    bool newTripActualy = false;
    bool newRoute = false;
    bool newRouteActive = false;
    bool deleteNewTrip = false;
    int nodesSelected = 0;
    bool drawOriginalRoute = true;

    std::size_t currentCarNode = std::size_t(-1);
    std::size_t newDestination = std::size_t(-1);

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
    CarController carController(carSprite, 100.f, carTextureUp, carTextureDown, carTextureLeft, carTextureRight, uiManager, routeManager, travelClock);
    uiManager.toggleAlgorithmOptions(true);
    uiManager.toggleStartOption(false);        
    uiManager.toggleRouteOptions(false);
    bool useDijkstra = true, startMovement = false, routeCalculated = false, algorithmSelected = false;
    bool showWeights = false;
    bool showStreets = false;
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

                    std::cout << "Tr�fico seleccionado: " << uiManager.trafficOptions[uiManager.selectedTrafficIndex] << std::endl;
                    selectingTrafficNodes = true;
                    trafficStartNode = std::size_t(-1);
                    trafficEndNode = std::size_t(-1);
                    carController.stopAtNextNode();
                    applyingTrafficChanges = true;
                }
                if (uiManager.assignAccidentButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))||
                    uiManager.coseviButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    awaitingStreetClose = true;
                    nodesSelected = 0;
                }

                if (awaitingStreetClose) {
                    std::size_t selectedNode = routeManager.findClosestNode(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                    if (selectedNode != std::numeric_limits<std::size_t>::max()) {
                        if (nodesSelected == 0) {
                            node1 = selectedNode;
                            nodesSelected++;
                        }
                        else if (nodesSelected == 1) {
                            if (routeManager.areNodesConnected(node1, selectedNode)) {
                                node2 = selectedNode;
                                awaitingStreetClose = false;

                                for (auto& street : map.getStreets()) {
                                    if ((street.getNode1() == node1 && street.getNode2() == node2) ||
                                        (street.getNode1() == node2 && street.getNode2() == node1)) {
                                        street.closeStreetDirection(node1, node2);

                                        if (uiManager.isDijkstraSelected()) {
                                            std::cout << "Recalculando con Dijkstra tras cerrar calle." << std::endl;
                                            routeManager.calculateRoute(true, floydWarshallResult);
                                        }
                                        else {
                                            std::cout << "Recalculando con Floyd-Warshall tras cerrar calle." << std::endl;
                                            floydWarshallResult = map.floydWarshall();
                                        }
                                    }
                                }
                                nodesSelected = 0;
                            }
                            else {
                                std::cout << "El segundo nodo no es adyacente al primero." << std::endl;
                            }
                        }
                    }
                }


                if (uiManager.openStreetButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    awaitingStreetOpen = true;
                    nodesSelected = 0;
                }



                if (uiManager.stopTripButton.getGlobalBounds().contains(mousePos) && !uiManager.isTripStopped) {
                    uiManager.showStopTripButton = false;
                    carController.stopAtNextNode();
                    carController.actualizarInicio(routeManager);
                    uiManager.isTripStopped = true;

                    std::size_t currentCarNode = carController.getCurrentNode(map);
                    std::size_t destinationNode = routeManager.getUpdatedEndNode();

                    if (currentCarNode == destinationNode) {
                        std::cout << "El carro ha llegado al destino. Mostrando solo botones 'Nuevo Viaje' y 'Limpiar'." << std::endl;

                        uiManager.showChangeRouteButton(false);
                        uiManager.showTrafficButtons(false);
                        uiManager.toggleRouteOptions(false);

                        uiManager.setShowNewTrip(true);
                        uiManager.setShowStartButton(false);
                    }
                    else {
                        std::cout << "El carro se ha detenido, pero no ha llegado al destino. Mostrando opciones de ruta." << std::endl;

                        uiManager.showChangeRouteButton(true);
                        newRouteActive = false;
                        uiManager.showTrafficButtons(true);
                        uiManager.toggleRouteOptions(true);
                        uiManager.setShowNewTrip(false);
                    }
                }


                if (awaitingStreetOpen) {
                    std::size_t selectedNode = routeManager.findClosestNode(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                    if (selectedNode != std::numeric_limits<std::size_t>::max()) {
                        if (nodesSelected == 0) {
                            node1 = selectedNode;
                            nodesSelected++;
                        }
                        else if (nodesSelected == 1) {
                            if (routeManager.areNodesConnected(node1, selectedNode)) {
                                node2 = selectedNode;
                                awaitingStreetOpen = false;

                                for (auto& street : map.getStreets()) {
                                    if ((street.getNode1() == node1 && street.getNode2() == node2) ||
                                        (street.getNode1() == node2 && street.getNode2() == node1)) {
                                        street.openStreetDirection(node1, node2);
                                        std::cout << "Calle entre " << node1 << " y " << node2 << " ha sido abierta." << std::endl;
                                        if (uiManager.isDijkstraSelected()) {
                                            std::cout << "Recalculando ruta con Dijkstra tras abrir calle." << std::endl;
                                            routeManager.calculateRoute(true, floydWarshallResult);
                                        }
                                        else {
                                            std::cout << "Recalculando ruta con Floyd-Warshall tras abrir calle." << std::endl;
                                            floydWarshallResult = map.floydWarshall();
                                            routeManager.calculateRoute(false, floydWarshallResult);
                                        }

                                        routeCalculated = true;
                                    }
                                }
                                nodesSelected = 0;
                            }
                            else {
                                std::cout << "El segundo nodo no es adyacente al primero." << std::endl;
                            }
                        }
                    }
                }

                if (uiManager.continueTripButton.getGlobalBounds().contains(mousePos) && uiManager.isTripStopped) {
                    std::size_t currentCarNode = carController.getCurrentNode(map);
                    routeManager.setStartNode(currentCarNode);
                    std::cout << "Nodo actual del auto al continuar viaje: " << currentCarNode << std::endl;

                    if (newTripActualy) {
                        std::size_t newDestination = routeManager.getUpdatedEndNode();
                        routeManager.calculateNewRoute(newDestination, currentCarNode, uiManager.isDijkstraSelected(), floydWarshallResult, carController.getPreviousAccumulatedWeight());
                        carController.startMovement(routeManager.getNewPath(), map, true, true);
                        std::cout << "Continuando viaje con el nuevo destino del nuevo viaje." << std::endl;
                    }
                    else if (newRoute && routeManager.isNewPathCalculated()) {
                        carController.startMovement(routeManager.getNewPath(), map, true, false);
                        std::cout << "Continuando viaje con la nueva ruta calculada (newPath)." << std::endl;
                    }
                    else {
                        std::size_t updatedEndNode = routeManager.getUpdatedEndNode();
                        routeManager.calculateNewRoute(updatedEndNode, currentCarNode, uiManager.isDijkstraSelected(), floydWarshallResult, carController.getPreviousAccumulatedWeight());
                        carController.startMovement(routeManager.getNewPath(), map, true, false);
                        std::cout << "Continuando viaje con la ruta recalculada hacia el destino original." << std::endl;
                    }

                    uiManager.isTripStopped = false;
                    newRoute = false;
                    newTripActualy = false;
                    uiManager.showChangeRouteButton(false);
                    uiManager.showTrafficButtons(false);
                    uiManager.showStopTripButton = true;
                }

                if (applyingTrafficChanges && carController.isStopped()) {
                    if (routeManager.isStartNodeSelected() && carController.hasValidRoute()) {
                        uiManager.setAlgorithmSelected(true);  

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

                                routeManager.calculateRoute(true, floydWarshallResult);

                                routeCalculated = true;
                                selectingTrafficNodes = false;
                                applyingTrafficChanges = false;
                                carController.stopAtNextNode();

                                std::cout << "Recalculando ruta usando Dijkstra tras cambio de tr�fico." << std::endl;
                            }
                        }
                    }
                }




                if (selectingTrafficNodes) {
                    std::size_t selectedNode = routeManager.findClosestNode(mousePos);
                    if (selectedNode != std::size_t(-1)) {
                        if (trafficStartNode == std::size_t(-1)) {
                            trafficStartNode = selectedNode;
                            std::cout << "Nodo de inicio para el tr�fico seleccionado: " << trafficStartNode << std::endl;
                        }
                        else if (trafficEndNode == std::size_t(-1) && selectedNode != trafficStartNode) {
                            trafficEndNode = selectedNode;
                            std::cout << "Nodo final para el tr�fico seleccionado: " << trafficEndNode << std::endl;

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
                                std::cout << "No se encontr� una calle entre los nodos seleccionados." << std::endl;
                            }
                        }
                    }
                }

                if (uiManager.changeRouteButton.getGlobalBounds().contains(mousePos)) {
                    std::cout << "Bot�n 'Cambiar ruta' presionado. El carro se detendr� en el siguiente nodo m�s cercano." << std::endl;

                    applyingTrafficChanges = false;
                    selectingTrafficNodes = false;
                    awaitingStreetClose = false;
                    awaitingStreetOpen = false;

                    trafficStartNode = std::size_t(-1);
                    trafficEndNode = std::size_t(-1);
                    carController.stopAtNextNode();
                    isChangingRoute = true;
                    uiManager.setTotalWeight(0.0f);
                    uiManager.setTotalCost(0.0f);
                    newRouteActive = true;
                    uiManager.showChangeRouteButton(false);
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
                                std::cout << "No se encontr� una calle entre los nodos seleccionados." << std::endl;
                            }
                        }
                    }
                }

        if (isChangingRoute) {
    std::size_t newDestination = routeManager.findClosestNode(mousePos);
    if (newDestination != std::size_t(-1)) {
        std::size_t currentCarNode = carController.getCurrentNode(map);

        bool useDijkstra = uiManager.isDijkstraSelected();
        auto floydWarshallResult = map.floydWarshall();

        routeManager.setTotalWeight(0.0f);
        uiManager.setTotalWeight(0.0f);
        uiManager.setTotalCost(0.0f);
        float previousAccumulatedWeight = carController.getPreviousAccumulatedWeight();
        routeManager.calculateNewRoute(newDestination, currentCarNode, useDijkstra, floydWarshallResult, previousAccumulatedWeight);

        const auto& newRouteNodes = routeManager.getNewPath();
        std::cout << "Nueva ruta calculada. Nodos de la nueva ruta: ";
        for (auto node : newRouteNodes) {
            std::cout << node << " ";
        }
        std::cout << std::endl;

        float totalWeight = routeManager.calculateTotalWeight(currentCarNode);
        float totalCost = routeManager.calculateTotalCost();
        uiManager.setShowStartButton(true);
        isChangingRoute = false;
        newRoute = true;
    }
}

                
        if (uiManager.newTripButton.getGlobalBounds().contains(mousePos)) {
            std::cout << "Bot�n 'Nuevo Viaje' presionado. Preparando para un nuevo destino." << std::endl;
            selectingNewDestination = true;
            uiManager.resetForNewTrip();
            routeManager.resetForNewTrip();
            carController.resetAccumulatedValues();
            uiManager.resetCostLabels();

            isSelectingNewTrip = true;
            readyToStartNewTrip = false;
            uiManager.setShowStartButton(true);
            newRoute = true;
            deleteNewTrip = true;
            uiManager.showNewTripButton = false;
            routeManager.resetRoute();

            drawOriginalRoute = false; 
        }




                if (isSelectingNewTrip) {
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
                            routeManager.calculateNewTrip(newDestination, currentCarNode, useDijkstra, floydWarshallResult);

                            float totalWeight = routeManager.calculateTotalWeight(currentCarNode);
                            float totalCost = routeManager.calculateTotalCost();

                            uiManager.setTotalWeight(0.0f);
                            uiManager.setTotalCost(0.0f);

                            isSelectingNewTrip = false;
                            newTrip = true;

                            uiManager.setShowStartButton(true);
                            uiManager.toggleStartOption(true);
                        }
                        else {
                            std::cerr << "Error: no se ha seleccionado una ruta v�lida." << std::endl;
                        }
                    }
                }




                if (uiManager.startButton.getGlobalBounds().contains(mousePos)) {
                    uiManager.showNewTripButton = false;
                    uiManager.toggleRouteOptions(false); 
                    uiManager.showStopTripButton = true;
                    std::cout << "Comienza el primer viaje." << std::endl;
                    if (newTrip) {
                        newTripActualy = true;
                        carController.startMovement(routeManager.getNewPath(), map, false, true);
                    }
                    else {
                        carController.startMovement(routeManager.getPath(), map, false, false);
                    }
                    uiManager.startClock();
                    startMovement = true;
                    uiManager.updateClock();

                    carVisible = true;
                    isTimerRunning = true;
                    travelClock.restart();
                    uiManager.setShowStartButton(false);

                    float totalWeight = routeManager.calculateTotalWeightUnique();
                    float totalCost = routeManager.calculateTotalCost();
                    newTrip = false;
                    uiManager.setTotalWeight(totalWeight);
                    uiManager.setTotalCost(totalCost);
                    uiManager.toggleStartOption(false);     
                    uiManager.toggleRouteOptions(true);
                }


                if (uiManager.clearButton.getGlobalBounds().contains(mousePos)) {
                    node1 = std::numeric_limits<std::size_t>::max();
                    node2 = std::numeric_limits<std::size_t>::max();
                    currentCarNode = std::size_t(-1);
                    newDestination = std::size_t(-1);
                    trafficStartNode = std::size_t(-1);
                    trafficEndNode = std::size_t(-1);
                    uiManager.showChangeRouteButton(false);
                    uiManager.isClockRunning = false;
                    uiManager.resetClock();
                    uiManager.isTripStopped=false;
                    nodesSelected = 0;
                    totalTimeCost = 0.0f;
                    totalCompleteCost = 0.0f;
                    timeCostPerSecond = 2.0f;
                    isTimerRunning = false;
                    startMovement = false;
                    routeCalculated = false;
                    algorithmSelected = false;
                    showWeights = false;
                    showStreets = false;
                    applyingTrafficChanges = false;
                    isChangingRoute = false;
                    selectingTrafficNodes = false;
                    awaitingNodeSelection = false;
                    awaitingStreetClose = false;
                    awaitingStreetOpen = false;
                    isSelectingNewTrip = false;
                    selectingNewDestination = false;
                    carVisible = false;
                    readyToStartNewTrip = false;
                    newTrip = false;
                    newTripActualy = false;
                    newRoute = false;
                    newRouteActive = false;
                    deleteNewTrip = false;
                    drawOriginalRoute = true;

                    uiManager.resetAlgorithmSelected();
                    uiManager.setTotalWeight(0.0f);
                    uiManager.setTotalCost(0.0f);
                    uiManager.selectedTrafficIndex = 0;
                    uiManager.selectedTrafficText.setString(uiManager.trafficOptions[0]);
                    uiManager.setShowCostLabels(false);
                    uiManager.toggleAlgorithmOptions(true);
                    uiManager.toggleStartOption(false);
                    uiManager.toggleRouteOptions(false);
                    uiManager.setShowStartButton(true);
                    uiManager.showNewTripButton = false;

                    routeManager.resetRoute();
                    routeManager.resetForNewTrip();
                    carController.resetAccumulatedValues();

                }

                if (uiManager.dijkstraCheckBox.getGlobalBounds().contains(mousePos)) {
                    algorithmSelected = true;
                    uiManager.setAlgorithmSelected(true);     
                    uiManager.toggleStartOption(true);        
                }
                else if (uiManager.floydCheckBox.getGlobalBounds().contains(mousePos)) {
                    algorithmSelected = true;
                    uiManager.setAlgorithmSelected(false);    
                    uiManager.toggleStartOption(true);       
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

        if (uiManager.getIsClockRunning()) {
            uiManager.updateClock();
        }


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
        
        if (drawOriginalRoute && !newTrip && !newTripActualy) {
            routeManager.drawRoute(window);
        }
        routeManager.drawNewRoute(window);

       
            routeManager.drawNewTrips(window);
        
        routeManager.drawClosedStreets(window);

            float totalWeight = routeManager.calculateTotalWeightUnique();
            routeManager.setTotalWeight(totalWeight);
            float totalCost = routeManager.calculateTotalCost();
        
        uiManager.drawUI(window);

        if (carVisible) {
            window.draw(carSprite);
        }

        window.display();
    }

    return 0;
}
