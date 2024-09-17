#include "CarController.h"
#include <cmath>

CarController::CarController(sf::Sprite& carSprite, float speed)
    : carSprite(carSprite), speed(speed), currentNodeInPath(0), moving(false) {}

void CarController::startMovement(const std::vector<std::size_t>& path, const Map& map) {
    if (!path.empty()) {
        this->path = path;
        currentNodeInPath = 0;
        carSprite.setPosition(map.getNodes()[path[0]].getPosition());
        moving = true;
    }
}


void CarController::update(float deltaTime, const Map& map) {
    if (moving && currentNodeInPath < path.size() - 1) {
        std::size_t currentNode = path[currentNodeInPath];
        std::size_t nextNode = path[currentNodeInPath + 1];
        sf::Vector2f currentPos = carSprite.getPosition();
        sf::Vector2f targetPos = map.getNodes()[nextNode].getPosition();
        sf::Vector2f direction = targetPos - currentPos;
        float distance = std::hypot(direction.x, direction.y);

        if (distance > 1.0f) {
            sf::Vector2f movement = direction / distance * speed * deltaTime;
            carSprite.move(movement);
        }
        else {
            currentNodeInPath++;
            if (currentNodeInPath >= path.size()) {
                moving = false; 
            }
        }
    }
}


void CarController::moveTowardsNextNode(sf::Vector2f start, sf::Vector2f end, float deltaTime) {
    sf::Vector2f direction = end - carSprite.getPosition();
    float distance = std::hypot(direction.x, direction.y);

    if (distance > 1.0f) {
        sf::Vector2f movement = direction / distance * speed * deltaTime;
        carSprite.move(movement);
    }
    else {
        currentNodeInPath++;
        if (currentNodeInPath >= path.size()) {
            moving = false;
        }
    }
}
