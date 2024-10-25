#include "Street.h"


Street::Street(std::size_t node1, std::size_t node2, const sf::Vector2f& pos1, const sf::Vector2f& pos2, bool isFixed, bool isBidirectional, float weight)
    : node1(node1), node2(node2), bidirectional(isBidirectional), weight(weight) {
    isClosed = false;
    line[0] = sf::Vertex(pos1, isBidirectional ? sf::Color::Blue : sf::Color::Red);
    line[1] = sf::Vertex(pos2, isBidirectional ? sf::Color::Blue : sf::Color::Red);
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

bool Street::isBidirectional() const {
    return bidirectional;
}
void Street::closeStreet()  {
    isClosed = true;
}

bool Street::isClosedDirection(std::size_t node1, std::size_t node2) const {
    if (this->node1 == node1 && this->node2 == node2) {
        return closedFromNode1ToNode2;
    }
    if (this->node1 == node2 && this->node2 == node1) {
        return closedFromNode2ToNode1;
    }
    return false;
}

void Street::closeStreetDirection(std::size_t fromNode, std::size_t toNode) {
    if (this->node1 == fromNode && this->node2 == toNode) {
        closedFromNode1ToNode2 = true;
    }
    if (this->node1 == toNode && this->node2 == fromNode) {
        closedFromNode2ToNode1 = true;
    }
}
void Street::openStreetDirection(std::size_t fromNode, std::size_t toNode) {
    if (this->node1 == fromNode && this->node2 == toNode) {
        closedFromNode1ToNode2 = false;
    }
    if (this->node1 == toNode && this->node2 == fromNode) {
        closedFromNode2ToNode1 = false;
    }
}
void Street::openStreet() {
    closedFromNode1ToNode2 = false;
    closedFromNode1ToNode2 = false;
}
std::size_t Street::getNode1() const {
    return node1;
}

std::size_t Street::getNode2() const {
    return node2;
}

sf::Vector2f Street::getStartPosition() const {
    return line[0].position;
}

sf::Vector2f Street::getEndPosition() const {
    return line[1].position;
}
