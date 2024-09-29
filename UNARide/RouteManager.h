#ifndef ROUTEMANAGER_H
#define ROUTEMANAGER_H

#include "Map.h"
#include <SFML/Graphics.hpp>
#include <vector>

class RouteManager {
public:
    RouteManager(Map& map, float costPerKm = 1.0f);

    void selectNode(sf::Vector2f mousePos);
    void calculateRoute(bool useDijkstra, const std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>>& floydWarshallResult);
    void resetRoute();
    float calculateTotalWeight() const;
    float calculateTotalCost() const;
    void updateCostPerKm();
    void calculateNewRoute(std::size_t newDestination, std::size_t currentCarNode);
    void drawRoute(sf::RenderWindow& window); 
    void drawNewRoute(sf::RenderWindow& window);
    void setEndNode(std::size_t newEndNode);
    // Solo una declaración de drawRoute
    const std::vector<std::size_t>& getPath() const;
    // RouteManager.h
    const std::vector<std::size_t>& getNewPath() const; // Para obtener la nueva ruta // Manejar nuevas rutas

    std::vector<std::size_t> calculateRouteFloydWarshall(
        const std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>>& floydWarshallResult,
        std::size_t newDestination);

    bool isStartNodeSelected() const;
    bool isEndNodeSelected() const;

    std::size_t getStartNode() const;
    std::size_t getEndNode() const;
    std::size_t findClosestNode(const sf::Vector2f& mousePos);

private:
    float costPerKm;

    bool startNodeSelected;
    bool endNodeSelected;
    bool routeCalculated;
    std::vector<std::size_t> originalPath; // Almacenar la ruta original
    std::vector<std::size_t> newPath;      // Almacenar la nueva ruta
    bool newPathCalculated;
    std::vector<std::size_t> path;
    Map& map;
    bool isNewRoute;
    std::size_t startNode;
    std::size_t endNode;
};

#endif
