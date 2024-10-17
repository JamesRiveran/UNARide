#ifndef CARCONTROLLER_H
#define CARCONTROLLER_H

#include <SFML/Graphics.hpp>
#include "UIManager.h"
#include "RouteManager.h"
#include "Map.h"
#include <vector>

class CarController {
public:
    CarController(sf::Sprite& carSprite, float speed, sf::Texture& upTexture, sf::Texture& downTexture,
        sf::Texture& leftTexture, sf::Texture& rightTexture, UIManager& uiManager, RouteManager& routeManager);

    void startMovement(const std::vector<std::size_t>& path, const Map& map, bool isNewRoute, bool isNewTrip);
    void update(float deltaTime, const Map& map);
    void stopMovement();
    void changeRoute(const std::vector<std::size_t>& newPath);
    void stopAtNextNode();
    std::size_t getCurrentNode(const Map& map);
    size_t findClosestNode(const sf::Vector2f& position, const std::vector<std::size_t>& path, const Map& map);
    bool isStopped() const;
    const std::vector<std::size_t>& getNodesTraversed() const { return nodesTraversed; } 
    bool hasValidRoute() const {
        return !path.empty();  
    }
    bool isAtDestination() const;

private:
    UIManager& uiManager;
    RouteManager& routeManager;
    void moveTowardsNextNode(sf::Vector2f start, sf::Vector2f end, float deltaTime);
    void updateCarDirection(const sf::Vector2f& direction);
    bool shouldStopAtNextNode = false;
    float speed;
    bool moving;
    float progress;
    bool isMoving;
    sf::Sprite& carSprite;
    std::vector<std::size_t> path;
    std::vector<std::size_t> originalPath;
    std::vector<std::size_t> nodesTraversed; 
    std::size_t currentNodeInPath;
    sf::Texture& upTexture;
    sf::Texture& downTexture;
    sf::Texture& leftTexture;
    sf::Texture& rightTexture;
};

#endif
