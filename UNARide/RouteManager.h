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
    void drawRoute(sf::RenderWindow& window);
    void resetRoute();
    float calculateTotalWeight() const;
    float calculateTotalCost() const;
    void updateCostPerKm();

    const std::vector<std::size_t>& getPath() const;

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

    std::vector<std::size_t> path;
    Map& map;

    std::size_t startNode;
    std::size_t endNode;
};

#endif
