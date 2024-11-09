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
    std::size_t getCurrentNode(const Map& map);
    size_t findClosestNode(const sf::Vector2f& position, const std::vector<std::size_t>& path, const Map& map);
    bool isStopped() const;
    const std::vector<std::size_t>& getNodesTraversed() const { return nodesTraversed; }
    bool hasValidRoute() const { return !path.empty(); }
    float getPreviousAccumulatedWeight() const { return previousAccumulatedWeight; } 
    bool isAtDestination() const;
    void stopAtNextNode();
    void continueMovement(bool useDijkstra, const std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>>& floydWarshallResult);
    void actualizarInicio(RouteManager& routeManager); // Nuevo método para actualizar el nodo de inicio
    std::size_t getCurrentNode() const; // Nuevo método para obtener el nodo actual
    void setPath(const std::vector<std::size_t>& newPath); // Nuevo método para actualizar la ruta del coche
    std::vector<std::size_t> nodosRecorridos; // Lista de nodos recorridos
    const std::vector<std::size_t>& getTraversedNodes() const {
        return traversedNodes;
    }
private:
    std::vector<std::size_t> traversedNodes;  // Declara aquí los nodos recorridos
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
};
#endif