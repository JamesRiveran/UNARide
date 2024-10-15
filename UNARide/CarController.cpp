#include "CarController.h"
#include <cmath>
#include <iostream>

CarController::CarController(sf::Sprite& carSprite, float speed, sf::Texture& upTexture, sf::Texture& downTexture,
    sf::Texture& leftTexture, sf::Texture& rightTexture, UIManager& uiManager, RouteManager& routeManager) 
    : carSprite(carSprite), speed(speed), upTexture(upTexture), downTexture(downTexture),
    leftTexture(leftTexture), rightTexture(rightTexture), currentNodeInPath(0), moving(false), progress(0.0f),
    uiManager(uiManager), routeManager(routeManager), isMoving(false) {} 



void CarController::startMovement(const std::vector<std::size_t>& path, const Map& map, bool isNewRoute) {
    if (!path.empty()) {
        this->path = path;
        currentNodeInPath = 0;

        carSprite.setPosition(map.getNodes()[path[currentNodeInPath]].getPosition());
        moving = true;
        progress = 0.0f;
        isMoving = true;
        uiManager.setCarroEnMovimiento(true);

        if (isNewRoute && !routeManager.hasChangedRoute) {
            routeManager.hasChangedRoute = true;
            routeManager.nodesSinceFirstChange.clear(); 
        }
    }
}


void CarController::updateCarDirection(const sf::Vector2f& direction) {
    float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159f;

    sf::Texture* newTexture = nullptr;
    std::string textureLabel;

    const float diagonalThreshold = 0.6f;

    if (std::abs(direction.x) > diagonalThreshold && std::abs(direction.y) > diagonalThreshold) {
        if (direction.x > 0 && direction.y > 0) {
            newTexture = &downTexture;
            textureLabel = "diagonal_down_right";
        }
        else if (direction.x < 0 && direction.y > 0) {
            newTexture = &downTexture;
            textureLabel = "diagonal_down_left";
        }
        else if (direction.x < 0 && direction.y < 0) {
            newTexture = &upTexture;
            textureLabel = "diagonal_up_left";
        }
        else {
            newTexture = &upTexture;
            textureLabel = "diagonal_up_right";
        }
    }
    else {
        if (std::abs(direction.x) > std::abs(direction.y)) {
            newTexture = (direction.x > 0) ? &rightTexture : &leftTexture;
            textureLabel = (direction.x > 0) ? "right" : "left";
        }
        else {
            newTexture = (direction.y > 0) ? &downTexture : &upTexture;
            textureLabel = (direction.y > 0) ? "down" : "up";
        }
    }

    carSprite.setTexture(*newTexture, true);
    carSprite.setOrigin(newTexture->getSize().x / 2.0f, newTexture->getSize().y / 2.0f);

    carSprite.setRotation((std::abs(direction.x) > diagonalThreshold && std::abs(direction.y) > diagonalThreshold) ? angle : 0);
}
void CarController::update(float deltaTime, const Map& map) {
    if (!moving || currentNodeInPath >= path.size() - 1) {
        moving = false;
        isMoving = false;
        uiManager.setCarroEnMovimiento(false);
        return;
    }

    std::size_t currentNode = path[currentNodeInPath];
    std::size_t nextNode = path[currentNodeInPath + 1];
    sf::Vector2f startPos = map.getNodes()[currentNode].getPosition();
    sf::Vector2f endPos = map.getNodes()[nextNode].getPosition();

    if (nodesTraversed.empty() || nodesTraversed.back() != currentNode) {
        nodesTraversed.push_back(currentNode);
    }

    sf::Vector2f direction = endPos - startPos;
    float distance = std::hypot(direction.x, direction.y);
    sf::Vector2f normalizedDirection = direction / distance;

    updateCarDirection(normalizedDirection);

    progress += speed * deltaTime / distance;

    if (progress >= 1.0f) {
        currentNodeInPath++;
        progress = 0.0f;

        if (shouldStopAtNextNode) {
            moving = false;
            isMoving = false;
            shouldStopAtNextNode = false;
            uiManager.setCarroEnMovimiento(false);
        }

        if (currentNodeInPath >= path.size() - 1) {
            moving = false;
            uiManager.setCarroEnMovimiento(false);
        }
    }
    else {
        carSprite.setPosition(startPos + direction * progress);
    }
}


void CarController::stopMovement() {
    isMoving = false;
    uiManager.setCarroEnMovimiento(false);
}

void CarController::changeRoute(const std::vector<std::size_t>& newPath) {
    if (isMoving) {
        path = newPath;  
        currentNodeInPath = 0;  
        progress = 0.0f;
        std::cout << "Ruta cambiada en tiempo real.\n";
    }
}

void CarController::moveTowardsNextNode(sf::Vector2f start, sf::Vector2f end, float deltaTime) {
    sf::Vector2f direction = end - carSprite.getPosition();
    float distance = std::hypot(direction.x, direction.y);

    if (distance > 1.0f) {
        carSprite.move(direction / distance * speed * deltaTime);
    }
    else {
        currentNodeInPath++;
        if (currentNodeInPath >= path.size()) {
            moving = false;
        }
    }
}

bool CarController::isStopped() const {
    return !isMoving;
}

void CarController::stopAtNextNode() {
    shouldStopAtNextNode = true; 
}

std::size_t CarController::getCurrentNode(const Map& map) {
    return path[currentNodeInPath];
}
