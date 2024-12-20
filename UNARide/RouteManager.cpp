#include "RouteManager.h"
#include <ctime>
#include <iostream>
#include <algorithm>

RouteManager::RouteManager(Map& map, float costPerKm)
    : map(map), startNodeSelected(false), endNodeSelected(false), routeCalculated(false),
    costPerKm(costPerKm), startNode(0), endNode(0), isNewRoute(false), newPathCalculated(false),
    hasChangedRoute(false), nodesSinceFirstChange(), totalWeight(0.0f)  
{
}

void printPredecessorMatrix(const std::vector<std::vector<int>>& pred) {
    std::cout << "Matriz de predecesores (Floyd-Warshall):\n";
    for (size_t i = 0; i < pred.size(); ++i) {
        for (size_t j = 0; j < pred[i].size(); ++j) {
            if (pred[i][j] == -1)
                std::cout << "INF ";
            else
                std::cout << pred[i][j] << " ";
        }
        std::cout << "\n";
    }
}

void RouteManager::selectNode(sf::Vector2f mousePos) {
    std::size_t clickedNode = findClosestNode(mousePos);
    if (clickedNode != std::size_t(-1)) {
        if (!startNodeSelected) {
            startNode = clickedNode;
            startNodeSelected = true;
            std::cout << "Nodo de inicio seleccionado: " << startNode << std::endl;
        }
        else if (!endNodeSelected && clickedNode != startNode) {
            endNode = clickedNode;
            endNodeSelected = true;
            std::cout << "Nodo final seleccionado: " << endNode << std::endl;
        }
    }
}
void RouteManager::resetForChangeRoute() {
    hasChangedRoute = false;
    newPathCalculated = false;
    nodesSinceFirstChange.clear();
    newPath.clear();
    std::cout << "Estados de RouteManager reseteados para cambio de ruta." << std::endl;
}


void RouteManager::drawRoute(sf::RenderWindow& window) {
    for (const auto& routePair : previousRoutes) {
        const auto& previousRoute = routePair.first;
        const sf::Color& routeColor = routePair.second;

        for (std::size_t i = 0; i < previousRoute.size() - 1; ++i) {
            std::size_t currentNode = previousRoute[i];
            std::size_t nextNode = previousRoute[i + 1];
            map.drawStreet(window, currentNode, nextNode, routeColor);  
        }
    }
}

void RouteManager::resetRoute() {
    startNodeSelected = false;
    endNodeSelected = false;
    routeCalculated = false;
    newPathCalculated = false;
    hasChangedRoute = false;

    path.clear();
    newPath.clear();
    previousRoutes.clear();
    newTrips.clear();
    nodesSinceFirstChange.clear();

    totalWeight = 0.0f;
    currentColorIndex = 0;

    std::cout << "Ruta y estado completamente reiniciados." << std::endl;
}


void RouteManager::clearNewTrips() {
    newTrips.clear();
}
float RouteManager::calculateTotalWeightUnique() const {
    return calculateTotalWeight(std::size_t(-1));
}

float RouteManager::calculateTotalWeight(std::size_t currentCarNode) const { 
    return calculateTotalWeight(currentCarNode, 0.0f);
} 

float RouteManager::calculateTotalWeight(std::size_t currentCarNode, float previousAccumulatedWeight) const {
    float totalWeight = previousAccumulatedWeight;  
    bool startCounting = false;

    if (path.size() > 1) {
        for (std::size_t i = 0; i < path.size() - 1; ++i) {
            std::size_t currentNode = path[i];
            std::size_t nextNode = path[i + 1];

            if (currentNode == currentCarNode) {
                startCounting = true;
            }

            if (startCounting) {
                bool streetFound = false;
                for (const auto& street : map.getStreets()) {
                    if ((street.getNode1() == currentNode && street.getNode2() == nextNode) ||
                        (street.isBidirectional() && street.getNode1() == nextNode && street.getNode2() == currentNode)) {

                        totalWeight += street.getWeight();
                        streetFound = true;
                        std::cout << "A�adiendo peso de la calle entre " << currentNode << " y " << nextNode
                           << ": " << street.getWeight() << " km" << std::endl;
                        break;
                    }
                }

                if (!streetFound) {
                    std::cout << "No se encontr� calle entre " << currentNode << " y " << nextNode << std::endl;
                }
            }
        }
    }

    if (newPath.size() > 1 && currentCarNode != std::size_t(-1)) {
        std::cout << "Calculando el peso desde el nodo actual " << currentCarNode << " hasta el nuevo destino.\n";
        for (std::size_t i = 0; i < newPath.size() - 1; ++i) {
            std::size_t currentNode = newPath[i];
            std::size_t nextNode = newPath[i + 1];

            bool streetFound = false;
            for (const auto& street : map.getStreets()) {
                if ((street.getNode1() == currentNode && street.getNode2() == nextNode) ||
                    (street.isBidirectional() && street.getNode1() == nextNode && street.getNode2() == currentNode)) {

                    totalWeight += street.getWeight();
                    streetFound = true;
                    std::cout << "A�adiendo peso de la calle en la nueva ruta entre " << currentNode
                        << " y " << nextNode << ": " << street.getWeight() << " km" << std::endl;
                    break;
                }
            }

            if (!streetFound) {
                std::cout << "No se encontr� calle en la nueva ruta entre " << currentNode << " y " << nextNode << std::endl;
            }
        }
    }

    const_cast<RouteManager*>(this)->totalWeight = totalWeight;
    return totalWeight;
}


void RouteManager::updateCostPerKm() {
    std::time_t currentTime = std::time(nullptr);
    std::tm localTime;
    localtime_s(&localTime, &currentTime);

    int hour = localTime.tm_hour;

    if (hour >= 6 && hour < 12) {
        costPerKm = 125.0f;
    }
    else if (hour >= 12 && hour < 18) {
        costPerKm = 150.0f;
    }
    else {
        costPerKm = 200.0f;
    }
}

float RouteManager::calculateTotalCost() const {
    float totalWeight = getTotalWeight();
    if (totalWeight == 0.0f) {
        return 0.0f;
    }

    std::cout << "Total weight: " << totalWeight << " km" << std::endl;
    std::cout << "Cost per km: " << costPerKm << " colones" << std::endl;
    float totalCost = totalWeight * costPerKm;
    std::cout << "Total cost: " << totalCost << " colones" << std::endl;
    return totalCost;
}


void RouteManager::calculateRoute(bool useDijkstra, const std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>>& floydWarshallResult) {
    if (startNodeSelected && endNodeSelected) {
        if (useDijkstra) {
            path = map.dijkstra(startNode, endNode);
        }
        else {
            const auto& pred = floydWarshallResult.second;
            if (pred[startNode][endNode] == -1) {
                std::cerr << "No se puede encontrar un camino v�lido." << std::endl;
                return;
            }

            path.clear();
            std::size_t current = endNode;
            while (current != startNode) {
                path.push_back(current);
                current = pred[startNode][current];
            }
            path.push_back(startNode);
            std::reverse(path.begin(), path.end());
        }

        if (previousRoutes.empty()) {
            previousRoutes.push_back({ path, sf::Color::Black });
            std::cout << "Ruta original guardada en color negro." << std::endl;
        }

        routeCalculated = true;
    }
}

std::size_t RouteManager::findClosestNode(const sf::Vector2f& mousePos) {
    float minDistance = 10.0f;
    std::size_t closestNode = -1;

    for (std::size_t i = 0; i < map.getNodes().size(); ++i) {
        float distance = std::hypot(mousePos.x - map.getNodes()[i].getPosition().x,
            mousePos.y - map.getNodes()[i].getPosition().y);
        if (distance < minDistance) {
            minDistance = distance;
            closestNode = i;
        }
    }
    return closestNode;
}

void RouteManager::calculateNewRoute(std::size_t newDestination, std::size_t currentCarNode, bool useDijkstra,
    const std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>>& floydWarshallResult,
    float previousAccumulatedWeight) {

    if (!nodesSinceFirstChange.empty()) {
        auto it = std::find(nodesSinceFirstChange.begin(), nodesSinceFirstChange.end(), currentCarNode);
        if (it != nodesSinceFirstChange.end()) {
            nodesSinceFirstChange.erase(it + 1, nodesSinceFirstChange.end());
        }
    }

    if (nodesSinceFirstChange.empty() || nodesSinceFirstChange.back() != currentCarNode) {
        nodesSinceFirstChange.push_back(currentCarNode);
    }

    if (!nodesSinceFirstChange.empty()) {
        std::vector<std::size_t> traversedPath(nodesSinceFirstChange);
        previousRoutes.push_back({ traversedPath, sf::Color(186, 85, 211) });
        std::cout << "Parte recorrida de la ruta morada guardada en `previousRoutes`." << std::endl;
    }

    newPath.clear();

    if (useDijkstra) {
        newPath = map.dijkstra(currentCarNode, newDestination);
    }
    else {
        const auto& pred = floydWarshallResult.second;

        if (pred[currentCarNode][newDestination] == -1) {
            std::cerr << "No se puede encontrar un camino v�lido con Floyd-Warshall." << std::endl;
            return;
        }

        std::size_t current = newDestination;
        while (current != currentCarNode) {
            newPath.push_back(current);
            current = pred[currentCarNode][current];
        }
        newPath.push_back(currentCarNode);
        std::reverse(newPath.begin(), newPath.end());
    }

    newPathCalculated = true;
    hasChangedRoute = true;

    float totalWeight = calculateTotalWeight(currentCarNode, previousAccumulatedWeight);
    setTotalWeight(totalWeight);
    float totalCost = calculateTotalCost();

    std::cout << "Nueva ruta calculada. Peso total: " << totalWeight << " km, Costo total: " << totalCost << " colones." << std::endl;
}

bool RouteManager::areNodesConnected(std::size_t node1, std::size_t node2) {
    for (const auto& street : map.getStreets()) {
        if ((street.getNode1() == node1 && street.getNode2() == node2) ||
            (street.getNode1() == node2 && street.getNode2() == node1)) {
            return true; 
        }
    }
    return false; 
}
void RouteManager::drawNewRoute(sf::RenderWindow& window) {
    if (!newPath.empty()) {
        for (std::size_t i = 0; i < newPath.size() - 1; ++i) {
            std::size_t currentNode = newPath[i];
            std::size_t nextNode = newPath[i + 1];
            map.drawStreet(window, currentNode, nextNode, sf::Color(186, 85, 211));
        }
    }

    if (hasChangedRoute && !nodesSinceFirstChange.empty()) {
        for (std::size_t i = 0; i < nodesSinceFirstChange.size() - 1; ++i) {
            std::size_t currentNode = nodesSinceFirstChange[i];
            std::size_t nextNode = nodesSinceFirstChange[i + 1];
            map.drawStreet(window, currentNode, nextNode, sf::Color(186, 85, 211));
        }
    }
}


float RouteManager::calculateWeightFromCurrentToEnd(std::size_t currentCarNode) const {
    float weightToRemove = 0.0f;

    bool startCounting = false;
    if (path.size() > 1) {
        for (std::size_t i = 0; i < path.size() - 1; ++i) {
            std::size_t currentNode = path[i];
            std::size_t nextNode = path[i + 1];

            if (currentNode == currentCarNode) {
                startCounting = true;
            }

            if (startCounting) {
                for (const auto& street : map.getStreets()) {
                    if ((street.getNode1() == currentNode && street.getNode2() == nextNode) ||
                        (street.isBidirectional() && street.getNode1() == nextNode && street.getNode2() == currentNode)) {
                        weightToRemove += street.getWeight();
                        break;
                    }
                }
            }
        }
    }

    return weightToRemove;
}
void RouteManager::drawNewTrips(sf::RenderWindow& window) {
    for (const auto& trip : newTrips) {
        const auto& newTripPath = trip.first;
        const sf::Color& tripColor = trip.second;

        for (std::size_t i = 0; i < newTripPath.size() - 1; ++i) {
            std::size_t currentNode = newTripPath[i];
            std::size_t nextNode = newTripPath[i + 1];
            map.drawStreet(window, currentNode, nextNode, sf::Color::Blue);
        }
    }
}


void RouteManager::drawClosedStreets(sf::RenderWindow& window) {
    for (const auto& street : map.getStreets()) {
        if (street.isClosedDirection(street.getNode1(), street.getNode2())) {
            map.drawStreet(window, street.getNode1(), street.getNode2(), sf::Color::Yellow);
        }
        if (street.isClosedDirection(street.getNode2(), street.getNode1())) {
            map.drawStreet(window, street.getNode2(), street.getNode1(), sf::Color::Yellow);
        }
    }
}



void RouteManager::calculateNewTrip(std::size_t newDestination, std::size_t currentCarNode, bool useDijkstra, const std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>>& floydWarshallResult) {
    if (!path.empty() && previousRoutes.empty()) {
        previousRoutes.push_back({ path, sf::Color::Black });
        std::cout << "Ruta original guardada en color negro." << std::endl;
    }

    if (!newPath.empty()) {
        previousRoutes.push_back({ newPath, sf::Color(186, 85, 211) }); 
    }

    newPath.clear();

    if (useDijkstra) {
        newPath = map.dijkstra(currentCarNode, newDestination);
    }
    else {
        const auto& pred = floydWarshallResult.second;
        if (pred[currentCarNode][newDestination] == -1) {
            std::cerr << "No se puede encontrar un camino v�lido con Floyd-Warshall." << std::endl;
            return;
        }

        std::size_t current = newDestination;
        while (current != currentCarNode) {
            newPath.push_back(current);
            current = pred[currentCarNode][current];
        }
        newPath.push_back(currentCarNode);
        std::reverse(newPath.begin(), newPath.end());
    }

    newPathCalculated = true;
    newTrips.push_back({ newPath, sf::Color::Blue });

    float totalWeight = calculateTotalWeight(currentCarNode);
    setTotalWeight(totalWeight);
    float totalCost = calculateTotalCost();
    if (!nodesSinceFirstChange.empty() && !newPath.empty()) {
        std::size_t lastNode = nodesSinceFirstChange.back();
        std::size_t firstNewNode = newPath.front();

        if (lastNode != firstNewNode) {
            previousRoutes.push_back({ {lastNode, firstNewNode}, sf::Color(186, 85, 211) });
            std::cout << "Conectando el �ltimo nodo recorrido (" << lastNode << ") con el primer nodo de la nueva ruta (" << firstNewNode << ")." << std::endl;
        }
    }

    std::cout << "Nuevo peso total de la ruta: " << totalWeight << " km" << std::endl;
    std::cout << "Nuevo costo total del viaje: " << totalCost << " colones" << std::endl;
}




std::size_t RouteManager::getStartNode() const {
    return startNode;
}

std::size_t RouteManager::getEndNode() const {
    return endNode;
}

const std::vector<std::size_t>& RouteManager::getPath() const {
    return path;
}

bool RouteManager::isStartNodeSelected() const {
    return startNodeSelected;
}

bool RouteManager::isEndNodeSelected() const {
    return endNodeSelected;
}

const std::vector<std::size_t>& RouteManager::getNewPath() const {
    return newPath;
}
void RouteManager::setTotalWeight(float weight) {
    totalWeight = weight;
}

float RouteManager::getCostPerKm() const {
    return costPerKm;
}


float RouteManager::getTotalWeight() const {
    return totalWeight;
}

void RouteManager::setStartNode(std::size_t newStartNode) {
    startNode = newStartNode;
    startNodeSelected = true; 
    std::cout << "Nodo de inicio actualizado a: " << startNode << std::endl;
}

bool RouteManager::isNewPathCalculated() const {
    return newPathCalculated;
}

std::size_t RouteManager::getUpdatedEndNode() const {
    return !newPath.empty() ? newPath.back() : endNode;
}

void RouteManager::resetForNewTrip() {
    clearRoutes();
    startNodeSelected = false;
    endNodeSelected = false;
    routeCalculated = false;
    hasChangedRoute = false;
    previousRoutes.clear();  
    std::cout << "Reset completado para un nuevo viaje. Rutas previas eliminadas." << std::endl;
}


void RouteManager::clearRoutes() {
    previousRoutes.clear();
    newPath.clear();      
    nodesSinceFirstChange.clear(); 
    newTrips.clear();       
    totalWeight = 0.0f;  
    hasChangedRoute = false; 
    newPathCalculated = false; 

    std::cout << "Rutas y datos reiniciados para un nuevo viaje." << std::endl;
}
