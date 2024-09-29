#ifndef CARCONTROLLER_H
#define CARCONTROLLER_H

#include <SFML/Graphics.hpp>
#include "UIManager.h"
#include "Map.h"
#include <vector>

class CarController {
public:
    CarController(sf::Sprite& carSprite, float speed, sf::Texture& upTexture, sf::Texture& downTexture,
        sf::Texture& leftTexture, sf::Texture& rightTexture, UIManager& uiManager);

    void startMovement(const std::vector<std::size_t>& path, const Map& map, bool isNewRoute); // Manejar nuevas rutas
    void update(float deltaTime, const Map& map);
    void stopMovement();
    void changeRoute(const std::vector<std::size_t>& newPath);
    void stopAtNextNode();
    std::size_t getCurrentNode(const Map& map);
   
private:
    void moveTowardsNextNode(sf::Vector2f start, sf::Vector2f end, float deltaTime);
    void updateCarDirection(const sf::Vector2f& direction);
    bool shouldStopAtNextNode = false;
    sf::Sprite& carSprite;
    std::vector<std::size_t> path;           // Ruta actual
    std::vector<std::size_t> originalPath;   // Ruta original
    std::size_t currentNodeInPath;
    float speed;
    bool moving;
    float progress;
   // Nueva bandera

    sf::Texture& upTexture;
    sf::Texture& downTexture;
    sf::Texture& leftTexture;
    sf::Texture& rightTexture;
    UIManager& uiManager;
    bool isMoving;
};

#endif
