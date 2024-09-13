#ifndef NODE_H
#define NODE_H

#include <SFML/Graphics.hpp>

class Node {
public:
    Node(const sf::Vector2f& pos, float radius, bool isFixed);

    sf::CircleShape& getCircle() const;
    sf::Vector2f getPosition() const;

private:
    sf::CircleShape circle;
    sf::Vector2f position;
    bool fixed;
};

#endif
