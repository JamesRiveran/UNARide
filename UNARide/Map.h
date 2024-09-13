#ifndef MAP_H
#define MAP_H

#include <vector>
#include "Node.h"
#include "Street.h"
#include <unordered_map>
#include <queue>

class Map {
public:
    Map();
    void saveNodesAndStreets(const std::string& filename) const;
    void loadNodesAndStreets(const std::string& filename);
    std::vector<std::size_t> dijkstra(std::size_t start, std::size_t goal);

    void draw(sf::RenderWindow& window);
    const std::vector<Node>& getNodes() const;
    const std::vector<Street>& getStreets() const;

private:
    std::vector<Node> nodes;
    std::vector<Street> streets;
};

#endif
