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

    void startMovement(const std::vector<std::size_t>& path, const Map& map, bool isNewRoute); 
    void update(float deltaTime, const Map& map);
    void stopMovement();
    void changeRoute(const std::vector<std::size_t>& newPath);
    void stopAtNextNode();
    std::size_t getCurrentNode(const Map& map);
   
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
    std::size_t currentNodeInPath;
    sf::Texture& upTexture;
    sf::Texture& downTexture;
    sf::Texture& leftTexture;
    sf::Texture& rightTexture;
    
  
};

#endif
