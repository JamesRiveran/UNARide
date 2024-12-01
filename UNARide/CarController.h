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
        sf::Texture& leftTexture, sf::Texture& rightTexture, UIManager& uiManager, RouteManager& routeManager, sf::Clock& travelClock);

    void startMovement(const std::vector<std::size_t>& path, const Map& map, bool isNewRoute, bool isNewTrip);
    void update(float deltaTime, const Map& map);
    void stopMovement();
    void changeRoute(const std::vector<std::size_t>& newPath);
    std::size_t getCurrentNode(const Map& map);
    size_t findClosestNode(const sf::Vector2f& position, const std::vector<std::size_t>& path, const Map& map);
    bool isStopped() const;
    const std::vector<std::size_t>& getNodesTraversed() const { return nodesTraversed; }
    bool hasValidRoute() const { return !path.empty(); }
    float getPreviousAccumulatedWeight() const { return previousAccumulatedWeight; } 
    bool isAtDestination() const;
    void stopAtNextNode();
    void continueMovement(bool useDijkstra, const std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>>& floydWarshallResult);
    void actualizarInicio(RouteManager& routeManager); 
    std::size_t getCurrentNode() const;
    void setPath(const std::vector<std::size_t>& newPath);
    std::vector<std::size_t> nodosRecorridos; 
    const std::vector<std::size_t>& getTraversedNodes() const {
        return traversedNodes;
    }
    void resetAccumulatedValues();
    bool isCarMoving() const;
    void setCarroEnMovimiento(bool enMovimiento);
private:
    std::vector<std::size_t> traversedNodes; 
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
    bool finalDestinationReached;
    bool shouldCalculateTotals;
    float accumulatedWeight;
    float previousAccumulatedWeight; 
    bool useDijkstra;
    std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>> floydWarshallResult;
    bool isTimerRunning = false;
    float totalTimeCost = 0.0f;
    float timeCostPerSecond = 2.0f;
    float totalCompleteCost = 0.0f;
    sf::Clock& travelClock; // Añadir esta línea
    double timeCost; // Declaración de timeCost

};
#endif