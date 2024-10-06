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
    float calculateTotalWeight(std::size_t currentCarNode) const;
    float calculateTotalCost() const;
    void updateCostPerKm();
    void drawRoute(sf::RenderWindow& window);
    void setTotalWeight(float weight);
    float getTotalWeight() const;
    const std::vector<std::size_t>& getPath() const;
    bool isStartNodeSelected() const;
    bool isEndNodeSelected() const;
    std::vector<std::size_t> calculateRouteFloydWarshall(
    const std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>>& floydWarshallResult,std::size_t newDestination);
    std::size_t getStartNode() const;
    std::size_t getEndNode() const;
    std::size_t findClosestNode(const sf::Vector2f& mousePos);
    bool areNodesConnected(std::size_t node1, std::size_t node2);

    //calculate new route
    float calculateWeightFromCurrentToEnd(std::size_t currentCarNode) const;
    void calculateNewRoute(std::size_t newDestination, std::size_t currentCarNode, bool useDijkstra, const std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>>& floydWarshallResult);
    void drawNewRoute(sf::RenderWindow& window);
    void setEndNode(std::size_t newEndNode);
    const std::vector<std::size_t>& getNewPath() const;
    std::vector<std::size_t> nodesSinceFirstChange;
    bool hasChangedRoute = false;

private:
    float costPerKm;
    float totalWeight;
    bool startNodeSelected;
    bool endNodeSelected;
    bool routeCalculated;
    bool newPathCalculated;
    bool isNewRoute;
   
    Map& map;

    std::vector<std::size_t> originalPath;
    std::vector<std::size_t> newPath;     
    std::vector<std::size_t> path;
    std::size_t startNode;
    std::size_t endNode;

};

#endif
