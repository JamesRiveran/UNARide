#include "Map.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <limits>
#include "iostream"

Map::Map() {}

void Map::saveNodesAndStreets(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not create or open the file '" << filename << "'." << std::endl;
        return;
    }

    file << "Nodes\n";
    for (const auto& node : nodes) {
        file << node.getPosition().x << ' ' << node.getPosition().y << '\n';
    }

    file << "Streets\n";
    for (const auto& street : streets) {
        file << street.getNode1() << ' ' << street.getNode2() << '\n';
    }

    std::cout << "Nodes and streets successfully saved to '" << filename << "'." << std::endl;
}

void Map::loadNodesAndStreets(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Could not open the file to load nodes and streets." << std::endl;
        return;
    }

    std::string line;
    bool loadingNodes = false;
    while (std::getline(file, line)) {
        if (line == "Nodes") {
            loadingNodes = true;
        }
        else if (line == "Streets") {
            loadingNodes = false;
        }
        else if (loadingNodes) {
            std::istringstream iss(line);
            float x, y;
            if (iss >> x >> y) {
                nodes.emplace_back(sf::Vector2f(x, y), 5.f, true);
            }
        }
        else {
            std::istringstream iss(line);
            std::size_t node1, node2;
            int directionality;
            if (iss >> node1 >> node2 >> directionality && node1 < nodes.size() && node2 < nodes.size()) {
                bool isBidirectional = (directionality == 1);
                streets.emplace_back(node1, node2, nodes[node1].getPosition(), nodes[node2].getPosition(), true, isBidirectional);
            }
        }
    }
}

std::vector<std::size_t> Map::dijkstra(std::size_t start, std::size_t goal) {
    std::vector<float> distances(nodes.size(), std::numeric_limits<float>::infinity());
    std::vector<std::size_t> predecessors(nodes.size(), std::numeric_limits<std::size_t>::max());
    std::vector<bool> visited(nodes.size(), false);

    distances[start] = 0;

    for (std::size_t i = 0; i < nodes.size(); ++i) {
        std::size_t current = std::numeric_limits<std::size_t>::max();
        float minDist = std::numeric_limits<float>::infinity();

        for (std::size_t j = 0; j < nodes.size(); ++j) {
            if (!visited[j] && distances[j] < minDist) {
                current = j;
                minDist = distances[j];
            }
        }

        if (current == std::numeric_limits<std::size_t>::max()) break;

        visited[current] = true;

        if (current == goal) break;

        for (const auto& street : streets) {
            if (street.getNode1() == current || (street.isBidirectional() && street.getNode2() == current)) {
                std::size_t neighbor = (current == street.getNode1()) ? street.getNode2() : street.getNode1();
                float distance = std::hypot(
                    nodes[neighbor].getPosition().x - nodes[current].getPosition().x,
                    nodes[neighbor].getPosition().y - nodes[current].getPosition().y
                );

                if (distances[neighbor] > distances[current] + distance) {
                    distances[neighbor] = distances[current] + distance;
                    predecessors[neighbor] = current;
                }
            }
        }
    }

    std::vector<std::size_t> path;
    for (std::size_t at = goal; at != start; at = predecessors[at]) {
        path.push_back(at);
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());

    return path;
}

void Map::draw(sf::RenderWindow& window) {
    for (const auto& street : streets) {
        window.draw(street.getLine().data(), 2, sf::Lines);
    }
    for (const auto& node : nodes) {
        window.draw(node.getCircle());
    }
}

void Map::drawStreet(sf::RenderWindow& window, std::size_t startNode, std::size_t endNode) const {
    if (startNode >= nodes.size() || endNode >= nodes.size()) {
        return;  
    }

    sf::Vector2f p1 = nodes[startNode].getPosition();
    sf::Vector2f p2 = nodes[endNode].getPosition();

    sf::VertexArray line(sf::Quads, 4);
    sf::Vector2f direction = p2 - p1;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    sf::Vector2f unitDir = direction / length;

    sf::Vector2f offset = sf::Vector2f(-unitDir.y, unitDir.x) * 2.5f;

    line[0].position = p1 + offset;
    line[1].position = p2 + offset;
    line[2].position = p2 - offset;
    line[3].position = p1 - offset;

    line[0].color = sf::Color::Black; 
    line[1].color = sf::Color::Black;
    line[2].color = sf::Color::Black;
    line[3].color = sf::Color::Black;

    window.draw(line);
}


const std::vector<Node>& Map::getNodes() const {
    return nodes;
}

const std::vector<Street>& Map::getStreets() const {
    return streets;
}
