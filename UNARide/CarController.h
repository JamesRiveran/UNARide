#ifndef CARCONTROLLER_H
#define CARCONTROLLER_H

#include <SFML/Graphics.hpp>
#include "Map.h"

class CarController {
public:
    CarController(sf::Sprite& carSprite, float speed, sf::Texture& upTexture, sf::Texture& downTexture, sf::Texture& leftTexture, sf::Texture& rightTexture);

    void startMovement(const std::vector<std::size_t>& path, const Map& map);
    void update(float deltaTime, const Map& map);

private:
    void moveTowardsNextNode(sf::Vector2f start, sf::Vector2f end, float deltaTime);
    void updateCarDirection(const sf::Vector2f& direction);

    sf::Sprite& carSprite;
    std::vector<std::size_t> path;
    std::size_t currentNodeInPath;
    float speed;
    bool moving;
    float progress;

    sf::Texture& upTexture;
    sf::Texture& downTexture;
    sf::Texture& leftTexture;
    sf::Texture& rightTexture;
};

#endif 
