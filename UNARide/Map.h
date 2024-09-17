#ifndef MAP_H
#define MAP_H

#include <vector>
#include "Node.h"
#include "Street.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <queue>

class Map {
public:
    Map();
    void saveNodesAndStreets(const std::string& filename) const;
    void loadNodesAndStreets(const std::string& filename);
    std::vector<std::size_t> dijkstra(std::size_t start, std::size_t goal);

    void draw(sf::RenderWindow& window);
    void drawStreet(sf::RenderWindow& window, std::size_t startNode, std::size_t endNode) const; 

    const std::vector<Node>& getNodes() const;
    const std::vector<Street>& getStreets() const;

private:
    std::vector<Node> nodes;
    std::vector<Street> streets;
};

#endif
