#ifndef MAP_H
#define MAP_H

#include <vector>
#include "Node.h"
#include "Street.h"
#include <SFML/Graphics.hpp>

class Map {
public:
    Map();
    void saveNodesAndStreets(const std::string& filename) const;
    void loadNodesAndStreets(const std::string& filename);
    std::vector<std::size_t> dijkstra(std::size_t start, std::size_t goal);
    std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>> floydWarshall();

    void draw(sf::RenderWindow& window);
    void drawStreet(sf::RenderWindow& window, std::size_t startNode, std::size_t endNode, sf::Color color = sf::Color::Black) const;

    void drawWeights(sf::RenderWindow& window, sf::Font& font);
    const Street* getStreetBetweenNodes(std::size_t node1, std::size_t node2) const;

    const std::vector<Node>& getNodes() const;
    const std::vector<Street>& getStreets() const;
    std::vector<Street>& getStreets(); 
private:
    std::vector<Node> nodes;
    std::vector<Street> streets;
};

#endif
