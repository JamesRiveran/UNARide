#include "Street.h"

Street::Street(std::size_t node1, std::size_t node2, const sf::Vector2f& pos1, const sf::Vector2f& pos2, bool isFixed, bool isBidirectional, float weight)
    : node1(node1), node2(node2), bidirectional(isBidirectional), weight(weight), closedFromNode1ToNode2(false), closedFromNode2ToNode1(false) {
    line[0] = sf::Vertex(pos1, sf::Color::Black);
    line[1] = sf::Vertex(pos2, sf::Color::Black);
}

float Street::getWeight() const {
    return weight;
}

void Street::setWeight(float newWeight) {
    weight = newWeight;
}

const std::array<sf::Vertex, 2>& Street::getLine() const {
    return line;
}

std::size_t Street::getNode1() const {
    return node1;
}

std::size_t Street::getNode2() const {
    return node2;
}

bool Street::isBidirectional() const {
    return bidirectional;
}

void Street::closeStreet() {
    closedFromNode1ToNode2 = true;
    closedFromNode2ToNode1 = true;
}

void Street::openStreet() {
    closedFromNode1ToNode2 = false;
    closedFromNode2ToNode1 = false;
}

void Street::closeStreetDirection(std::size_t fromNode, std::size_t toNode) {
    if (fromNode == node1 && toNode == node2) {
        closedFromNode1ToNode2 = true;
    }
    else if (fromNode == node2 && toNode == node1) {
        closedFromNode2ToNode1 = true;
    }
}

void Street::openStreetDirection(std::size_t fromNode, std::size_t toNode) {
    if (fromNode == node1 && toNode == node2) {
        closedFromNode1ToNode2 = false;
    }
    else if (fromNode == node2 && toNode == node1) {
        closedFromNode2ToNode1 = false;
    }
}

bool Street::isClosedDirection(std::size_t node1, std::size_t node2) const {
    if (node1 == this->node1 && node2 == this->node2) {
        return closedFromNode1ToNode2;
    }
    else if (node1 == this->node2 && node2 == this->node1) {
        return closedFromNode2ToNode1;
    }
    return false;
}
