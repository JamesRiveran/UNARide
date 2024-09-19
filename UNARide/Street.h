#ifndef STREET_H
#define STREET_H

#include <SFML/Graphics.hpp>
#include <array>

class Street {
public:
    Street(std::size_t node1, std::size_t node2, const sf::Vector2f& pos1, const sf::Vector2f& pos2, bool isFixed, bool isBidirectional, float weight);

    const std::array<sf::Vertex, 2>& getLine() const;
    bool isBidirectional() const;
    std::size_t getNode1() const;
    std::size_t getNode2() const;
    sf::Vector2f getStartPosition() const;
    sf::Vector2f getEndPosition() const;
    float getWeight() const;  

private:
    std::size_t node1;
    std::size_t node2;
    std::array<sf::Vertex, 2> line;
    bool bidirectional;
    float weight; 
};

#endif
