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
    float calculateTotalWeight(std::size_t currentCarNode, float previousAccumulatedWeight) const;
    float calculateTotalCost() const;
    void updateCostPerKm();
    void drawRoute(sf::RenderWindow& window);
    void drawNewTrips(sf::RenderWindow& window); 
    void setTotalWeight(float weight);
    float getTotalWeight() const;
    const std::vector<std::size_t>& getPath() const;
    bool isStartNodeSelected() const;
    bool isEndNodeSelected() const;
    std::size_t getStartNode() const;
    std::size_t getEndNode() const;
    std::size_t findClosestNode(const sf::Vector2f& mousePos);
    bool areNodesConnected(std::size_t node1, std::size_t node2);
    std::vector<std::pair<std::vector<std::size_t>, sf::Color>> previousRoutes;
    std::vector<std::pair<std::vector<std::size_t>, sf::Color>> newTrips; 
    bool isNewPathCalculated() const;
    std::size_t getUpdatedEndNode() const;
    std::vector<sf::Color> routeColors = {
        sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow, sf::Color::Magenta
    };
    int currentColorIndex = 0;
    float calculateTotalWeightUnique() const;
    float calculateTotalWeight(std::size_t currentCarNode) const;
    float calculateWeightFromCurrentToEnd(std::size_t currentCarNode) const;
    void calculateNewRoute(std::size_t newDestination, std::size_t currentCarNode, bool useDijkstra,
        const std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>>& floydWarshallResult,
        float previousAccumulatedWeight);
    void calculateNewTrip(std::size_t newDestination, std::size_t currentCarNode, bool useDijkstra, const std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>>& floydWarshallResult);
    void drawNewRoute(sf::RenderWindow& window);
    void setEndNode(std::size_t newEndNode);
    void drawClosedStreets(sf::RenderWindow& window);
    const std::vector<std::size_t>& getNewPath() const;
    std::vector<std::size_t> nodesSinceFirstChange;
    bool hasChangedRoute = false;
    float getCostPerKm() const;
    void clearNewTrips();
    void setStartNode(std::size_t newStartNode);
    void drawTraversedPath(sf::RenderWindow& window, const std::vector<std::size_t>& traversedNodes);


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
