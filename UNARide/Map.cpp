#include <iomanip> 
#include "Map.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>

Map::Map() {}

void Map::drawWeights(sf::RenderWindow& window, sf::Font& font) {
    for (const auto& street : streets) {
        sf::Vector2f startPos = nodes[street.getNode1()].getPosition();
        sf::Vector2f endPos = nodes[street.getNode2()].getPosition();

        sf::Vector2f midPoint((startPos.x + endPos.x) / 2.0f, (startPos.y + endPos.y) / 2.0f);

        std::ostringstream weightStream;
        weightStream << std::fixed << std::setprecision(2) << street.getWeight();  

        sf::Text weightText;
        weightText.setFont(font);
        weightText.setString(weightStream.str()); 
        weightText.setCharacterSize(14);  
        weightText.setFillColor(sf::Color::Black); 
        weightText.setPosition(midPoint);  

        window.draw(weightText);
    }
}

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
        file << street.getNode1() << ' ' << street.getNode2() << ' ' << (street.isBidirectional() ? 1 : 0) << ' ' << street.getWeight() << '\n';
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
            float weight;
            int directionality;
            if (iss >> node1 >> node2 >> directionality >> weight && node1 < nodes.size() && node2 < nodes.size()) {
                bool isBidirectional = (directionality == 1);
                streets.emplace_back(node1, node2, nodes[node1].getPosition(), nodes[node2].getPosition(), true, isBidirectional, weight);
            }
        }
    }
}

std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>> Map::floydWarshall() {
    std::size_t numNodes = nodes.size();

    std::vector<std::vector<float>> dist(numNodes, std::vector<float>(numNodes, std::numeric_limits<float>::infinity()));
    std::vector<std::vector<int>> pred(numNodes, std::vector<int>(numNodes, -1));

    for (const auto& street : streets) {
        dist[street.getNode1()][street.getNode2()] = street.getWeight();
        pred[street.getNode1()][street.getNode2()] = street.getNode1();

        if (street.isBidirectional()) {
            dist[street.getNode2()][street.getNode1()] = street.getWeight();
            pred[street.getNode2()][street.getNode1()] = street.getNode2();
        }
    }

    for (std::size_t i = 0; i < numNodes; ++i) {
        dist[i][i] = 0.0f;
        pred[i][i] = i;
    }

    for (std::size_t k = 0; k < numNodes; ++k) {
        for (std::size_t i = 0; i < numNodes; ++i) {
            for (std::size_t j = 0; j < numNodes; ++j) {
                if (dist[i][k] < std::numeric_limits<float>::infinity() && dist[k][j] < std::numeric_limits<float>::infinity()) {
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        pred[i][j] = pred[k][j];
                    }
                }
            }
        }
    }

    return { dist, pred }; 
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
            std::size_t neighbor = (current == street.getNode1()) ? street.getNode2() : street.getNode1();

            // Verifica si la calle está cerrada en la dirección específica antes de usarla
            if ((street.getNode1() == current && !street.isClosedDirection(street.getNode1(), street.getNode2())) ||
                (street.getNode2() == current && !street.isClosedDirection(street.getNode2(), street.getNode1()))) {

                float distance = street.getWeight();

                if (distances[neighbor] > distances[current] + distance) {
                    distances[neighbor] = distances[current] + distance;
                    predecessors[neighbor] = current;
                }
            }
        }
    }

    std::vector<std::size_t> path;
    for (std::size_t at = goal; at != start; at = predecessors[at]) {
        if (predecessors[at] == std::numeric_limits<std::size_t>::max()) {
            return {};
        }
        path.push_back(at);
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());

    return path;
}


const Street* Map::getStreetBetweenNodes(std::size_t node1, std::size_t node2) const {
    for (const auto& street : streets) {
        if ((street.getNode1() == node1 && street.getNode2() == node2) ||
            (street.getNode1() == node2 && street.getNode2() == node1)) {
            return &street; 
        }
    }
    return nullptr; 
}


void Map::draw(sf::RenderWindow& window) {
    for (const auto& street : streets) {
        window.draw(street.getLine().data(), 2, sf::Lines);
    }
    for (const auto& node : nodes) {
        window.draw(node.getCircle());
    }
}

void Map::drawStreet(sf::RenderWindow& window, std::size_t startNode, std::size_t endNode, sf::Color color) const {
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

    line[0].color = color;
    line[1].color = color;
    line[2].color = color;
    line[3].color = color;

    window.draw(line);
}


const std::vector<Node>& Map::getNodes() const {
    return nodes;
}

const std::vector<Street>& Map::getStreets() const {
    return streets;
}

std::vector<Street>& Map::getStreets() {
    return streets;
}
