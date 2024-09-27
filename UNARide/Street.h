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
    bool isBidirectional() const;
    std::size_t getNode1() const;
    std::size_t getNode2() const;
    sf::Vector2f getStartPosition() const;
    sf::Vector2f getEndPosition() const;

private:
    std::size_t node1, node2;
    bool bidirectional;
    float weight;
    std::array<sf::Vertex, 2> line;
};

#endif
