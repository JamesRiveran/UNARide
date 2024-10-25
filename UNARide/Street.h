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
    
    bool isClosed;
    bool closedFromNode1ToNode2=false;
    bool closedFromNode2ToNode1=false;
    
    
    sf::Vector2f getStartPosition() const;
    sf::Vector2f getEndPosition() const;

    void closeStreet();
    bool isClosedDirection(std::size_t node1, std::size_t node2) const;
        
    bool isStreetClosed() const {
        return isClosed;
    }
    void closeStreetDirection(std::size_t fromNode, std::size_t toNode);
    void openStreetDirection(std::size_t fromNode, std::size_t toNode);
    Street() : closedFromNode1ToNode2(false), closedFromNode2ToNode1(false) {}


    void closeStreetFromNode1ToNode2() {
        closedFromNode1ToNode2 = true;
    }

    void closeStreetFromNode2ToNode1() {
        closedFromNode2ToNode1 = true;
    }
    bool isClosedInDirection(std::size_t node1, std::size_t node2) const {
        if (node1 == getNode1() && node2 == getNode2()) {
            return closedFromNode1ToNode2;
        }
        else if (node1 == getNode2() && node2 == getNode1()) {
            return closedFromNode2ToNode1;
        }
        return false;
    }

    void openStreet();

private:
   

    std::size_t node1, node2;
    bool bidirectional;
   
    float weight;
    std::array<sf::Vertex, 2> line;
};

#endif
