#include "Node.h"

Node::Node(const sf::Vector2f& pos, float radius, bool isFixed)
    : circle(radius), position(pos), fixed(isFixed) {
    circle.setPosition(pos - sf::Vector2f(radius, radius));
    circle.setFillColor(sf::Color::Green);
}

sf::CircleShape& Node::getCircle() const {
    return const_cast<sf::CircleShape&>(circle);
}

sf::Vector2f Node::getPosition() const {
    return position;
}
