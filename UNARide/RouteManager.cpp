#include "RouteManager.h"
#include <iostream>
#include <algorithm>

RouteManager::RouteManager(Map& map) : map(map), startNodeSelected(false), endNodeSelected(false), routeCalculated(false) {}

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

            //printPredecessorMatrix(pred); Print matrix predecessors

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
            map.drawStreet(window, currentNode, nextNode);
        }
    }
}

void RouteManager::resetRoute() {
    startNodeSelected = false;
    endNodeSelected = false;
    routeCalculated = false;
    path.clear();
    std::cout << "Ruta reiniciada." << std::endl;
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

const std::vector<std::size_t>& RouteManager::getPath() const {
    return path;
}

bool RouteManager::isStartNodeSelected() const {
    return startNodeSelected;
}

bool RouteManager::isEndNodeSelected() const {
    return endNodeSelected;
}
