#ifndef ROUTEMANAGER_H
#define ROUTEMANAGER_H

#include "Map.h"
#include <SFML/Graphics.hpp>
#include <vector>

class RouteManager {
public:
    RouteManager(Map& map);

    void selectNode(sf::Vector2f mousePos);
    void calculateRoute(bool useDijkstra, const std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>>& floydWarshallResult);
    void drawRoute(sf::RenderWindow& window);
    void resetRoute();

    const std::vector<std::size_t>& getPath() const;

    bool isStartNodeSelected() const;
    bool isEndNodeSelected() const;

private:
    std::size_t findClosestNode(const sf::Vector2f& mousePos);

    bool startNodeSelected;
    bool endNodeSelected;
    bool routeCalculated;

    std::vector<std::size_t> path;
    Map& map;

    std::size_t startNode;
    std::size_t endNode;
};

#endif
