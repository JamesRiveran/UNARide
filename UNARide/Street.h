#ifndef STREET_H
#define STREET_H

#include <SFML/Graphics.hpp>
#include <array>

class Street {
public:
    Street(std::size_t node1, std::size_t node2, const sf::Vector2f& pos1, const sf::Vector2f& pos2, bool isFixed, bool isBidirectional, float weight);

    float getWeight() const;
    void setWeight(float newWeight);
    const std::array<sf::Vertex, 2>& getLine() const;
    std::size_t getNode1() const;
    std::size_t getNode2() const;

    bool isBidirectional() const;

    void closeStreet();
    void openStreet();
    void closeStreetDirection(std::size_t fromNode, std::size_t toNode);
    void openStreetDirection(std::size_t fromNode, std::size_t toNode);

    bool isClosedDirection(std::size_t node1, std::size_t node2) const;

    Street() : node1(0), node2(0), bidirectional(false), weight(0.0f), closedFromNode1ToNode2(false), closedFromNode2ToNode1(false) {}

private:
    std::size_t node1, node2;
    bool bidirectional;
    float weight;
    std::array<sf::Vertex, 2> line;

    bool closedFromNode1ToNode2;
    bool closedFromNode2ToNode1;
};

#endif 
