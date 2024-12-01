#include "UIManager.h"
#include <sstream>
#include <iostream>

UIManager::UIManager(sf::RenderWindow& window, sf::Font& font) : showNewTripButton(false), showStartButton(true), isTripStopped(false) {
    float buttonWidth = 120.f;   
    float buttonHeight = 40.f;   
    float leftOffset = 20.f;     
    float initialYPosition = 180.f;
    float verticalSpacing = 10.f; 

    assignAccidentButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    assignAccidentButton.setFillColor(sf::Color::Black);
    assignAccidentButton.setOutlineColor(sf::Color::White);
    assignAccidentButton.setOutlineThickness(2.f);
    assignAccidentButton.setPosition(leftOffset, initialYPosition);
    assignAccidentButtonText.setFont(font);
    assignAccidentButtonText.setString("Accidente");
    assignAccidentButtonText.setCharacterSize(18);
    assignAccidentButtonText.setFillColor(sf::Color::White);
    centerTextInButton(assignAccidentButtonText, assignAccidentButton);

    coseviButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    coseviButton.setFillColor(sf::Color::Black);
    coseviButton.setOutlineColor(sf::Color::White);
    coseviButton.setOutlineThickness(2.f);
    coseviButton.setPosition(leftOffset, initialYPosition + buttonHeight + verticalSpacing);
    coseviButtonText.setFont(font);
    coseviButtonText.setString("COSEVI");
    coseviButtonText.setCharacterSize(18);
    coseviButtonText.setFillColor(sf::Color::White);
    centerTextInButton(coseviButtonText, coseviButton);

    openStreetButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    openStreetButton.setFillColor(sf::Color::Black);
    openStreetButton.setOutlineColor(sf::Color::White);
    openStreetButton.setOutlineThickness(2.f);
    openStreetButton.setPosition(leftOffset, initialYPosition + 2 * (buttonHeight + verticalSpacing));
    openStreetText.setFont(font);
    openStreetText.setString("Abrir calle");
    openStreetText.setCharacterSize(18);
    openStreetText.setFillColor(sf::Color::White);
    centerTextInButton(openStreetText, openStreetButton);

    clearButton.setSize(sf::Vector2f(100.f, 40.f));
    clearButton.setFillColor(sf::Color::Black);
    clearButton.setOutlineColor(sf::Color::White);
    clearButton.setOutlineThickness(2.f);

    startButton.setSize(sf::Vector2f(100.f, 40.f));
    startButton.setFillColor(sf::Color::Black);
    startButton.setOutlineColor(sf::Color::White);
    startButton.setOutlineThickness(2.f);

    dijkstraCheckBox.setSize(sf::Vector2f(20.f, 20.f));
    dijkstraCheckBox.setFillColor(sf::Color::White);
    dijkstraCheckBox.setOutlineColor(sf::Color::Black);
    dijkstraCheckBox.setOutlineThickness(2.f);

    floydCheckBox.setSize(sf::Vector2f(20.f, 20.f));
    floydCheckBox.setFillColor(sf::Color::White);
    floydCheckBox.setOutlineColor(sf::Color::Black);
    floydCheckBox.setOutlineThickness(2.f);

    clearButtonText.setFont(font);
    clearButtonText.setString("Limpiar");
    clearButtonText.setCharacterSize(18);
    clearButtonText.setFillColor(sf::Color::White);

    startButtonText.setFont(font);
    startButtonText.setString("Iniciar");
    startButtonText.setCharacterSize(18);
    startButtonText.setFillColor(sf::Color::White);

    dijkstraText.setFont(font);
    dijkstraText.setString("Dijkstra");
    dijkstraText.setCharacterSize(18);
    dijkstraText.setFillColor(sf::Color::Black);

    floydText.setFont(font);
    floydText.setString("Floyd-Warshall");
    floydText.setCharacterSize(18);
    floydText.setFillColor(sf::Color::Black);

    toggleWeightsButton.setSize(sf::Vector2f(100.f, 40.f));
    toggleWeightsButton.setFillColor(sf::Color::Black);
    toggleWeightsButton.setOutlineColor(sf::Color::White);
    toggleWeightsButton.setOutlineThickness(2.f);

    toggleWeightsButtonText.setFont(font);
    toggleWeightsButtonText.setString("Pesos");
    toggleWeightsButtonText.setCharacterSize(18);
    toggleWeightsButtonText.setFillColor(sf::Color::White);

    toggleStreetsButton.setSize(sf::Vector2f(100.f, 40.f));
    toggleStreetsButton.setFillColor(sf::Color::Black);
    toggleStreetsButton.setOutlineColor(sf::Color::White);
    toggleStreetsButton.setOutlineThickness(2.f);

    toggleStreetsButtonText.setFont(font);
    toggleStreetsButtonText.setString("Calles");
    toggleStreetsButtonText.setCharacterSize(18);
    toggleStreetsButtonText.setFillColor(sf::Color::White);

    totalWeightText.setFont(font);
    totalWeightText.setCharacterSize(18);
    totalWeightText.setFillColor(sf::Color::Black);
    totalWeightText.setPosition(20.f, window.getSize().y - 120.f);

    totalCostText.setFont(font);
    totalCostText.setCharacterSize(18);
    totalCostText.setFillColor(sf::Color::Black);
    totalCostText.setPosition(20.f, window.getSize().y - 90.f);

    changeRouteButton.setSize(sf::Vector2f(130.f, 40.f));
    changeRouteButton.setFillColor(sf::Color::Black);
    changeRouteButton.setOutlineColor(sf::Color::White);
    changeRouteButton.setOutlineThickness(2.f);

    changeRouteButtonText.setFont(font);
    changeRouteButtonText.setString("Cambiar ruta");
    changeRouteButtonText.setCharacterSize(18);
    changeRouteButtonText.setFillColor(sf::Color::White);
    centerTextInButton(changeRouteButtonText, changeRouteButton);

    newTripButton.setSize(sf::Vector2f(130.f, 40.f));
    newTripButton.setFillColor(sf::Color::Black);
    newTripButton.setOutlineColor(sf::Color::White);
    newTripButton.setOutlineThickness(2.f);

    newTripButtonText.setFont(font);
    newTripButtonText.setString("Nuevo Viaje");
    newTripButtonText.setCharacterSize(18);
    newTripButtonText.setFillColor(sf::Color::White);
    newTripButton.setPosition(10.f, 300.f);
    centerTextInButton(newTripButtonText, newTripButton);

    stopTripButton.setSize(sf::Vector2f(130.f, 40.f));
    stopTripButton.setFillColor(sf::Color::Black);
    stopTripButton.setOutlineColor(sf::Color::White);
    stopTripButton.setOutlineThickness(2.f);

    stopTripButtonText.setFont(font);
    stopTripButtonText.setString("Detener viaje");
    stopTripButtonText.setCharacterSize(18);
    stopTripButtonText.setFillColor(sf::Color::White);

    continueTripButton.setSize(sf::Vector2f(130.f, 40.f));
    continueTripButton.setFillColor(sf::Color::Black);
    continueTripButton.setOutlineColor(sf::Color::White);
    continueTripButton.setOutlineThickness(2.f);

    continueTripButtonText.setFont(font);
    continueTripButtonText.setString("Continuar viaje");
    continueTripButtonText.setCharacterSize(18);
    continueTripButtonText.setFillColor(sf::Color::White);

    stopTripButton.setPosition(window.getSize().x - 150.f, 180.f);
    continueTripButton.setPosition(window.getSize().x - 150.f, 240.f);
    centerTextInButton(stopTripButtonText, stopTripButton);
    centerTextInButton(continueTripButtonText, continueTripButton);

    initializeComboBox(font);
    resizeUI(window);
    initializeClockDisplay(font, window);
    elapsedTimeInSeconds = 0;
    costPerSecond = 2.0f;

    timeElapsedText.setFont(font);
    timeElapsedText.setCharacterSize(18);
    timeElapsedText.setFillColor(sf::Color::Black);
    timeElapsedText.setPosition(20.f, 20.f);


    totalCompleteCostText.setFont(font);
    totalCompleteCostText.setCharacterSize(18);
    totalCompleteCostText.setFillColor(sf::Color::Black);
    totalCompleteCostText.setPosition(window.getSize().x / 2 + 20.f, window.getSize().y - 90.f); 
    timeElapsedText.setFont(font);
    timeElapsedText.setCharacterSize(18);
    timeElapsedText.setFillColor(sf::Color::Black);
    timeElapsedText.setPosition(window.getSize().x / 2 - -307, 440.f);
    timeElapsedText.setString("00:00");
    isClockRunning = false;
    elapsedTimeInSeconds = 0;

    timeCostText.setFont(font);
    timeCostText.setCharacterSize(18);
    timeCostText.setFillColor(sf::Color::Black);
    timeCostText.setPosition(221.f, window.getSize().y - 80.f);
    timeCostText.setString("Costo Tiempo:");

    totalCompleteCostText.setFont(font);
    totalCompleteCostText.setCharacterSize(18);
    totalCompleteCostText.setFillColor(sf::Color::Black);
    totalCompleteCostText.setPosition(221.f, window.getSize().y - 40.f);
    totalCompleteCostText.setString("Total a pagar:");
    showTrafficOptions = false;
}


void UIManager::initializeComboBox(sf::Font& font) {
    trafficOptions = { "Normal", "Moderado", "Lento" };
    selectedTrafficText.setFont(font);
    selectedTrafficText.setCharacterSize(18);
    selectedTrafficText.setFillColor(sf::Color::Black);
    selectedTrafficText.setString(trafficOptions[selectedTrafficIndex]);

    trafficComboBox.setSize(sf::Vector2f(120.f, 30.f));
    trafficComboBox.setFillColor(sf::Color::White);
    trafficComboBox.setOutlineColor(sf::Color::Black);
    trafficComboBox.setOutlineThickness(2.f);
}

void UIManager::resizeUI(sf::RenderWindow& window) {
    float rightOffset = window.getSize().x - 140.f; 
    float leftOffset = 10.f; 

    clearButton.setPosition(leftOffset, 450.f);
    centerTextInButton(clearButtonText, clearButton);

    startButton.setPosition(rightOffset, 20.f);
    centerTextInButton(startButtonText, startButton);

    changeRouteButton.setPosition(rightOffset, 70.f);
    centerTextInButton(changeRouteButtonText, changeRouteButton);

    stopTripButton.setPosition(rightOffset, 120.f); 
    centerTextInButton(stopTripButtonText, stopTripButton);

    continueTripButton.setPosition(rightOffset, 170.f); 
    centerTextInButton(continueTripButtonText, continueTripButton);

    newTripButton.setPosition(rightOffset, 220.f);
    centerTextInButton(newTripButtonText, newTripButton);

    dijkstraCheckBox.setPosition(leftOffset, 20.f);
    floydCheckBox.setPosition(leftOffset, 60.f);
    dijkstraText.setPosition(50.f, 20.f);
    floydText.setPosition(50.f, 60.f);

    trafficComboBox.setPosition(leftOffset, 120.f);
    selectedTrafficText.setPosition(trafficComboBox.getPosition().x + 10.f, trafficComboBox.getPosition().y + 5.f);

    assignAccidentButton.setPosition(leftOffset, 180.f);
    centerTextInButton(assignAccidentButtonText, assignAccidentButton);

    openStreetButton.setPosition(leftOffset, 300.f);
    centerTextInButton(openStreetText, openStreetButton);

    coseviButton.setPosition(leftOffset, 240.f);
    centerTextInButton(coseviButtonText, coseviButton);

    toggleWeightsButton.setPosition(rightOffset, window.getSize().y - 60.f);
    centerTextInButton(toggleWeightsButtonText, toggleWeightsButton);

    toggleStreetsButton.setPosition(rightOffset, window.getSize().y - 120.f);
    centerTextInButton(toggleStreetsButtonText, toggleStreetsButton);

    totalWeightText.setPosition(leftOffset, window.getSize().y - 80.f);
    totalCostText.setPosition(leftOffset, window.getSize().y - 40.f);
    timeElapsedText.setPosition(leftOffset, window.getSize().y - 100.f);
    timeCostText.setPosition(leftOffset, window.getSize().y - 70.f);
    totalCompleteCostText.setPosition(leftOffset, window.getSize().y - 40.f);
}



void UIManager::drawUI(sf::RenderWindow& window) {
    if (showAlgorithmOptions) {
        window.draw(dijkstraCheckBox);
        window.draw(floydCheckBox);
        window.draw(dijkstraText);
        window.draw(floydText);
    }

    if (showStartButton && showStartOption) {
        window.draw(startButton);
        window.draw(startButtonText);
    }

    window.draw(clearButton);
    window.draw(clearButtonText);
    window.draw(toggleWeightsButton);
    window.draw(toggleWeightsButtonText);
    window.draw(toggleStreetsButton);
    window.draw(toggleStreetsButtonText);

    

    if (showRouteOptions) {
        if (isTripStopped) {
            window.draw(trafficComboBox);
            window.draw(selectedTrafficText);

            window.draw(assignAccidentButton);
            window.draw(assignAccidentButtonText);
            window.draw(openStreetButton);
            window.draw(openStreetText);
            window.draw(coseviButton);
            window.draw(coseviButtonText);
            window.draw(continueTripButton);
            window.draw(continueTripButtonText);
        }
        
        if (carroEnMovimiento && !isTripStopped) {
            window.draw(stopTripButton);
            window.draw(stopTripButtonText);
        }

        if (!carroEnMovimiento) {
            window.draw(totalWeightText);
            window.draw(totalCostText);
        }
        if (showNewTripButton) {
            window.draw(newTripButton);
            window.draw(newTripButtonText);
        }
    }
    if (showChangeRoute) {
        window.draw(changeRouteButton);
        window.draw(changeRouteButtonText);
    }
    window.draw(timeElapsedText);

    if (showCostLabels) {
        window.draw(totalWeightText);
        window.draw(totalCostText);
        window.draw(timeCostText);
        window.draw(totalCompleteCostText);
    }
}


void UIManager::isCarMoving(sf::RenderWindow& window, bool carroEnMovimiento) {
    if (carroEnMovimiento) {
        window.draw(changeRouteButton);
        window.draw(changeRouteButtonText);
    }
}

int UIManager::getTrafficMultiplier() const {
    switch (selectedTrafficIndex) {
    case 0: return 1;
    case 1: return 2;
    case 2: return 3;
    default: return 1;
    }
}

void UIManager::setAlgorithmSelected(bool isDijkstra) {
    if (isDijkstra) {
        dijkstraCheckBox.setFillColor(sf::Color::Black);
        floydCheckBox.setFillColor(sf::Color::White);
    }
    else {
        floydCheckBox.setFillColor(sf::Color::Black);
        dijkstraCheckBox.setFillColor(sf::Color::White);
    }
}

void UIManager::resetAlgorithmSelected() {
    dijkstraCheckBox.setFillColor(sf::Color::White);
    floydCheckBox.setFillColor(sf::Color::White);
}

void UIManager::centerTextInButton(sf::Text& text, const sf::RectangleShape& button) {
    sf::FloatRect textRect = text.getLocalBounds();
    sf::FloatRect buttonRect = button.getGlobalBounds();
    text.setPosition(
        buttonRect.left + (buttonRect.width / 2.0f) - (textRect.width / 2.0f),
        buttonRect.top + (buttonRect.height / 2.0f) - (textRect.height / 2.0f) - 5.f
    );
}



void UIManager::setTotalWeight(float totalWeight) {
    if (totalWeight == 0.0f) {
        totalWeightText.setString("");
    }
    else {
        std::ostringstream oss;
        oss << "Total Pesos: " << totalWeight;
        totalWeightText.setString(oss.str());
    }
}


    void UIManager::initializeClockDisplay(sf::Font & font, sf::RenderWindow & window) {
        timeElapsedText.setFont(font);
        timeElapsedText.setCharacterSize(18);
        timeElapsedText.setFillColor(sf::Color::Black);
        timeElapsedText.setPosition(20.f, window.getSize().y - 100.f);

        totalCompleteCostText.setFont(font);
        totalCompleteCostText.setCharacterSize(18);
        totalCompleteCostText.setFillColor(sf::Color::Black);
        totalCompleteCostText.setPosition(20.f, window.getSize().y - 40.f);

        timeElapsedText.setString("Tiempo transcurrido: 0 segundos");
        timeCostText.setString("Costo por tiempo: 0 colones");
        totalCompleteCostText.setString("Total completo a pagar: 0 colones");
    }


void UIManager::updateTimer() {
    elapsedTimeInSeconds = static_cast<int>(clock.getElapsedTime().asSeconds());
    std::ostringstream oss;
    oss << "Tiempo transcurrido: " << elapsedTimeInSeconds << " segundos";
    timeElapsedText.setString(oss.str());
}

void UIManager::updateTimeCost() {
    float timeCost = elapsedTimeInSeconds * costPerSecond;
    std::ostringstream oss;
    oss << "Costo Tiempo: " << timeCost;
    timeCostText.setString(oss.str());
}

void UIManager::updateTotalCompleteCost(float totalCost) {
    float timeCost = elapsedTimeInSeconds * costPerSecond;
    float completeTotalCost = totalCost + timeCost;
    std::ostringstream oss;
    oss << "Total a pagar: " << completeTotalCost;
    totalCompleteCostText.setString(oss.str());
}
void UIManager::setTotalTimeCost() {
    double timeCost = elapsedTimeInSeconds * costPerSecond; 
    std::ostringstream oss;
    oss << "Costo Tiempo: " << static_cast<int>(timeCost) << " colones";
    timeCostText.setString(oss.str());
}


void UIManager::setTotalCompleteCost(double totalCost) {
    std::ostringstream oss;
    oss << "Total a pagar: " << static_cast<int>(totalCost) << " colones";
    totalCompleteCostText.setString(oss.str());
}

void UIManager::setShowCostLabels(bool show) {
    showCostLabels = show;
}

void UIManager::showChangeRouteButton(bool show) {
    showChangeRoute = show;
}


void UIManager::setTotalCost(float totalCost) {
    if (totalCost == 0.0f) {
        totalCostText.setString("");
    }
    else {
        std::ostringstream oss;
        oss << "Costo Total: " << totalCost;
        totalCostText.setString(oss.str());
    }
}

void UIManager::setCarroEnMovimiento(bool enMovimiento) {
    carroEnMovimiento = enMovimiento;
}

bool UIManager::isDijkstraSelected() const {
    return dijkstraCheckBox.getFillColor() == sf::Color::Black;
}

void UIManager::setShowStartButton(bool value) {
    showStartButton = value;
}

bool UIManager::isShowStartButton() const {
    return showStartButton;
}

void UIManager::setShowNewTrip(bool value) {
    showNewTripButton = value;
}

bool UIManager::isShowNewTrip() const {
    return showNewTripButton;
}

void UIManager::toggleAlgorithmOptions(bool visible) {
    showAlgorithmOptions = visible;
}

void UIManager::toggleStartOption(bool visible) {
    showStartOption = visible;
}

void UIManager::toggleRouteOptions(bool visible) {
    showRouteOptions = visible;
}
void UIManager::startClock() {
    isClockRunning = true;
    clock.restart();
}

void UIManager::stopClock() {
    isClockRunning = false;
}

void UIManager::resetClock() {
    elapsedTimeInSeconds = 0;
    timeElapsedText.setString("00:00");
}

bool UIManager::getIsClockRunning() const {
    return isClockRunning;
}
void UIManager::updateClock() {
    if (isClockRunning) {
        elapsedTimeInSeconds = static_cast<int>(clock.getElapsedTime().asSeconds());
        int minutes = elapsedTimeInSeconds / 60;
        int seconds = elapsedTimeInSeconds % 60;

        std::ostringstream oss;
        oss << (minutes < 10 ? "0" : "") << minutes << ":"
            << (seconds < 10 ? "0" : "") << seconds;
        timeElapsedText.setString(oss.str());
    }
}
float UIManager::getCostPerSecond() const {
    return costPerSecond;
}
void UIManager::resetForNewTrip() {
    setTotalWeight(0.0f);
    setTotalCost(0.0f);
    setTotalTimeCost();
    setTotalCompleteCost(0.0f);
    setShowCostLabels(false);
    resetClock();
}
void UIManager::resetCostLabels() {
    setTotalWeight(0.0f);
    setTotalCost(0.0f);
    setTotalTimeCost();
    setTotalCompleteCost(0.0f);
    std::cout << "Etiquetas de costo y peso reiniciadas." << std::endl;
}
void UIManager::showTrafficButtons(bool show) {
    showTrafficOptions = show;
}

