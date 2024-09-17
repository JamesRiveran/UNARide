#include "RouteManager.h"
#include <cmath>
#include <iostream>

RouteManager::RouteManager(Map& map)
    : map(map), startNode(0), endNode(0), startNodeSelected(false), endNodeSelected(false), routeCalculated(false) {
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

void RouteManager::calculateRoute(bool useDijkstra) {
    if (startNodeSelected && endNodeSelected) {
        if (useDijkstra) {
            path = map.dijkstra(startNode, endNode);
        }
        else {
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
