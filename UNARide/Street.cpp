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
