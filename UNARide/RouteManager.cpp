#include "RouteManager.h"
#include <ctime>
#include <iostream>
#include <algorithm>

RouteManager::RouteManager(Map& map, float costPerKm)
    : map(map), startNodeSelected(false), endNodeSelected(false), routeCalculated(false),
    costPerKm(costPerKm), startNode(0), endNode(0), isNewRoute(false), newPathCalculated(false) {}


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

void RouteManager::calculateRoute(bool useDijkstra, const std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>>& floydWarshallResult) {
    if (startNodeSelected && endNodeSelected) {
        if (useDijkstra) {
            path = map.dijkstra(startNode, endNode);  
        }
        else {
            const auto& dist = floydWarshallResult.first;
            const auto& pred = floydWarshallResult.second;

            if (pred[startNode][endNode] == -1) {
                std::cerr << "No se puede encontrar un camino válido." << std::endl;
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

        routeCalculated = true;  
        std::cout << "Ruta calculada entre nodos " << startNode << " y " << endNode << std::endl;
    }
}
void RouteManager::drawRoute(sf::RenderWindow& window) {
    if (routeCalculated) {
        for (std::size_t i = 0; i < path.size() - 1; ++i) {
            std::size_t currentNode = path[i];
            std::size_t nextNode = path[i + 1];
            map.drawStreet(window, currentNode, nextNode, sf::Color::Black);
        }
    }
}
void RouteManager::drawNewRoute(sf::RenderWindow& window) {
    if (newPathCalculated && !newPath.empty()) {
        for (std::size_t i = 0; i < newPath.size() - 1; ++i) {
            std::size_t currentNode = newPath[i];
            std::size_t nextNode = newPath[i + 1];
            // Dibujar la calle entre los dos nodos en color magenta
            map.drawStreet(window, currentNode, nextNode, sf::Color::Magenta);
        }
    }
}



void RouteManager::resetRoute() {
    startNodeSelected = false;
    endNodeSelected = false;
    routeCalculated = false;
    newPathCalculated = false;  // Asegurarse de que la nueva ruta también se limpie
    path.clear();
    newPath.clear();  // Limpiar la nueva ruta (línea verde)
    std::cout << "Ruta reiniciada." << std::endl;
}

float RouteManager::calculateTotalWeight() const {
    float totalWeight = 0.0f;

    if (path.size() > 1) {
        for (std::size_t i = 0; i < path.size() - 1; ++i) {
            std::size_t currentNode = path[i];
            std::size_t nextNode = path[i + 1];

            for (const auto& street : map.getStreets()) {
                if ((street.getNode1() == currentNode && street.getNode2() == nextNode) ||
                    (street.isBidirectional() && street.getNode1() == nextNode && street.getNode2() == currentNode)) {
                    totalWeight += street.getWeight();
                    break;
                }
            }
        }
    }
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
    const_cast<RouteManager*>(this)->updateCostPerKm();
    return calculateTotalWeight() * costPerKm;
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
std::vector<std::size_t> RouteManager::calculateRouteFloydWarshall(
    const std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>>& floydWarshallResult,
    std::size_t newDestination) {

    const auto& dist = floydWarshallResult.first;
    const auto& pred = floydWarshallResult.second;
    std::vector<std::size_t> recalculatedPath;

    if (pred[startNode][newDestination] == -1) {
        std::cerr << "No se puede encontrar un camino válido hacia el nuevo destino." << std::endl;
        return recalculatedPath;
    }

    std::size_t current = newDestination;
    while (current != startNode) {
        recalculatedPath.push_back(current);
        current = pred[startNode][current];
    }
    recalculatedPath.push_back(startNode);
    std::reverse(recalculatedPath.begin(), recalculatedPath.end());

    std::cout << "Nueva ruta calculada hacia el nuevo destino: " << newDestination << std::endl;
    return recalculatedPath;
}
void RouteManager::setEndNode(std::size_t newEndNode) {
    endNode = newEndNode;
    endNodeSelected = true;
    std::cout << "Nuevo nodo final actualizado: " << endNode << std::endl;
}

void RouteManager::calculateNewRoute(std::size_t newDestination, std::size_t currentCarNode) {
    // Borra la ruta nueva anterior
    newPath.clear();

    // Usa Dijkstra para calcular la nueva ruta desde el nodo actual del carro hasta el nuevo destino
    newPath = map.dijkstra(currentCarNode, newDestination);

    // Marcar que se ha calculado una nueva ruta
    newPathCalculated = true;
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
