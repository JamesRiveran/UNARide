#include "CarController.h"
#include <cmath>
#include <iostream>

CarController::CarController(sf::Sprite& carSprite, float speed, sf::Texture& upTexture, sf::Texture& downTexture,
    sf::Texture& leftTexture, sf::Texture& rightTexture, UIManager& uiManager, RouteManager& routeManager)
    : carSprite(carSprite), speed(speed), upTexture(upTexture), downTexture(downTexture),
    leftTexture(leftTexture), rightTexture(rightTexture), currentNodeInPath(0), moving(false), progress(0.0f),
    uiManager(uiManager), routeManager(routeManager), isMoving(false), shouldStopAtNextNode(false),
    finalDestinationReached(false), shouldCalculateTotals(false), accumulatedWeight(0.0f),
    previousAccumulatedWeight(0.0f) 
{
}

void CarController::startMovement(const std::vector<std::size_t>& newRoute, const Map& map, bool isNewRoute, bool isNewTrip) {
    if (!newRoute.empty()) {
        if (isNewTrip) {
            // Solo reinicia `currentNodeInPath` si es un viaje nuevo
            currentNodeInPath = 0;
        }

        path = newRoute;
        if (currentNodeInPath < path.size()) {
            carSprite.setPosition(map.getNodes()[path[currentNodeInPath]].getPosition());
        }

        moving = true;
        progress = 0.0f;
        isMoving = true;
        uiManager.setCarroEnMovimiento(true);

        if (isNewRoute) {
            previousAccumulatedWeight = accumulatedWeight;
            accumulatedWeight = 0.0f;
            routeManager.hasChangedRoute = true;
        }

        shouldCalculateTotals = false;
        finalDestinationReached = false;

        std::cout << "Movimiento iniciado desde el nodo actual. Peso acumulado reiniciado a " << accumulatedWeight << " km." << std::endl;
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
        if (moving && currentNodeInPath >= path.size() - 1 && !finalDestinationReached) {
            moving = false;
            isMoving = false;
            uiManager.setCarroEnMovimiento(false);
            finalDestinationReached = true;

            if (finalDestinationReached) {
                float totalWeight = previousAccumulatedWeight + accumulatedWeight;
                if (totalWeight > 0) {
                    float totalCost = totalWeight * routeManager.getCostPerKm();
                    uiManager.setTotalWeight(totalWeight);
                    uiManager.setTotalCost(totalCost);
                    std::cout << "Llegada al destino final. Peso total acumulado (incluyendo rutas anteriores): "
                        << totalWeight << " km. Total a pagar: " << totalCost << " colones." << std::endl;
                }
            }
            return;
        }
        return;
    }

    std::size_t currentNode = path[currentNodeInPath];
    std::size_t nextNode = path[currentNodeInPath + 1];
    sf::Vector2f startPos = map.getNodes()[currentNode].getPosition();
    sf::Vector2f endPos = map.getNodes()[nextNode].getPosition();

    if (routeManager.hasChangedRoute) {
        if (routeManager.nodesSinceFirstChange.empty() || routeManager.nodesSinceFirstChange.back() != currentNode) {
            routeManager.nodesSinceFirstChange.push_back(currentNode);
        }
    }

    sf::Vector2f direction = endPos - startPos;
    float distance = std::hypot(direction.x, direction.y);
    sf::Vector2f normalizedDirection = direction / distance;

    updateCarDirection(normalizedDirection);

    progress += speed * deltaTime / distance;

    if (progress >= 1.0f) {
        // Almacenar el nodo actual en `traversedNodes` antes de avanzar al siguiente
        traversedNodes.push_back(currentNode);

        currentNodeInPath++;
        progress = 0.0f;

        actualizarInicio(routeManager);

        // Código para manejar el peso acumulado
        std::cout << "Intentando obtener calle entre nodo " << currentNode << " y nodo " << nextNode << "." << std::endl;
        const Street* street = map.getStreetBetweenNodes(currentNode, nextNode);

        if (street) {
            accumulatedWeight += street->getWeight();
            std::cout << "Calle encontrada entre nodo " << currentNode << " y nodo " << nextNode
                << ". Peso de la calle: " << street->getWeight() << " km. Peso acumulado actual: "
                << accumulatedWeight << " km." << std::endl;
        }
        else {
            std::cerr << "Error: No se encontró una calle entre nodo " << currentNode
                << " y nodo " << nextNode << "." << std::endl;
        }

        // Detener el carro en el siguiente nodo si `shouldStopAtNextNode` es verdadero
        if (shouldStopAtNextNode && moving) {
            moving = false;
            isMoving = false;
            shouldStopAtNextNode = false;
            uiManager.setCarroEnMovimiento(false);
            std::cout << "El carro se detiene en el siguiente nodo." << std::endl;
            return;
        }

        if (currentNodeInPath >= path.size() - 1 && !finalDestinationReached) {
            moving = false;
            isMoving = false;
            uiManager.setCarroEnMovimiento(false);

            float totalCost = (previousAccumulatedWeight + accumulatedWeight) * routeManager.getCostPerKm();
            uiManager.setTotalWeight(previousAccumulatedWeight + accumulatedWeight);
            uiManager.setTotalCost(totalCost);

            std::cout << "Llegada al destino final. Peso total acumulado: "
                << previousAccumulatedWeight + accumulatedWeight
                << " km. Total a pagar: " << totalCost << " colones." << std::endl;

            uiManager.showNewTripButton = true;
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
    if (isMoving && !newPath.empty()) {
        std::cout << "Ruta cambiada. Peso acumulado hasta el momento: " << accumulatedWeight << " km." << std::endl;

        // No se borra `traversedNodes`, solo se cambia la ruta.
        path = newPath;
        currentNodeInPath = 0;
        progress = 0.0f;

        shouldCalculateTotals = false;
        finalDestinationReached = false;
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
bool CarController::isAtDestination() const {
    return currentNodeInPath >= path.size() - 1;
}

bool CarController::isStopped() const {
    return !isMoving;
}

std::size_t CarController::getCurrentNode(const Map& map) {
    return path[currentNodeInPath];
}

void CarController::stopAtNextNode() {
    shouldStopAtNextNode = true;  // Señaliza detener en el siguiente nodo
    std::cout << "Carro se detendrá en el siguiente nodo." << std::endl;
}

void CarController::actualizarInicio(RouteManager& routeManager) {
    std::size_t nodoActual = getCurrentNode();
    routeManager.setStartNode(nodoActual); // Actualiza el nodo de inicio en el gestor de rutas
}
std::size_t CarController::getCurrentNode() const {
    return path[currentNodeInPath];
}

void CarController::continueMovement(bool useDijkstra, const std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>>& floydWarshallResult) {
    if (!isMoving && !path.empty()) {
        // Asegura que el nodo de inicio es el nodo actual, no el nodo de inicio original
        routeManager.setStartNode(getCurrentNode());

        routeManager.calculateRoute(useDijkstra, floydWarshallResult);

        // Establece la nueva ruta calculada
        setPath(routeManager.getPath());

        // Configura para continuar el movimiento
        isMoving = true;
        moving = true;
        shouldStopAtNextNode = false; // Asegura que la señal de detención esté desactivada
        uiManager.setCarroEnMovimiento(true);
        std::cout << "El carro continúa su viaje desde el nodo actual." << std::endl;
    }
}

void CarController::setPath(const std::vector<std::size_t>& newPath) {
    path = newPath;
    currentNodeInPath = 0; // Reinicia el progreso en la nueva ruta
    progress = 0.0f;       // Restablece el progreso del coche
}

