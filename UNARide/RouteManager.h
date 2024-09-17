#ifndef ROUTEMANAGER_H
#define ROUTEMANAGER_H

#include "Map.h"
#include <SFML/Graphics.hpp>
#include <vector>

class RouteManager {
public:
    RouteManager(Map& map);

    void selectNode(sf::Vector2f mousePos);
    void calculateRoute(bool useDijkstra);
    void drawRoute(sf::RenderWindow& window);
    void resetRoute();

    const std::vector<std::size_t>& getPath() const;

    bool isStartNodeSelected() const;
    bool isEndNodeSelected() const;

    bool startNodeSelected, endNodeSelected;

private:
    std::size_t findClosestNode(const sf::Vector2f& mousePos);

    Map& map;
    std::size_t startNode, endNode;
    bool routeCalculated;
    std::vector<std::size_t> path;
};

#endif
