#include "UIManager.h"
#include <sstream>
#include <iostream>

UIManager::UIManager(sf::RenderWindow& window, sf::Font& font) : showNewTripButton(false), showStartButton(true) {
    clearButton.setSize(sf::Vector2f(100.f, 40.f));
    clearButton.setFillColor(sf::Color::Black);
    clearButton.setOutlineColor(sf::Color::White);
    clearButton.setOutlineThickness(2.f);

    startButton.setSize(sf::Vector2f(100.f, 40.f));
    startButton.setFillColor(sf::Color::Black);
    startButton.setOutlineColor(sf::Color::White);
    startButton.setOutlineThickness(2.f);

    assignAccidentButton.setSize(sf::Vector2f(150.f, 40.f));
    assignAccidentButton.setFillColor(sf::Color::Black);
    assignAccidentButton.setOutlineColor(sf::Color::White);
    assignAccidentButton.setOutlineThickness(2.f);

    assignAccidentButtonText.setFont(font);
    assignAccidentButtonText.setString("Accidente");
    assignAccidentButtonText.setCharacterSize(18);
    assignAccidentButtonText.setFillColor(sf::Color::White);

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
    totalWeightText.setPosition(20.f, window.getSize().y - 80.f);

    totalCostText.setFont(font);
    totalCostText.setCharacterSize(18);
    totalCostText.setFillColor(sf::Color::Black);
    totalCostText.setPosition(20.f, window.getSize().y - 40.f);

    changeRouteButton.setSize(sf::Vector2f(100.f, 40.f));
    changeRouteButton.setFillColor(sf::Color::Black);
    changeRouteButton.setOutlineColor(sf::Color::White);
    changeRouteButton.setOutlineThickness(2.f);

    changeRouteButtonText.setFont(font);
    changeRouteButtonText.setString("Cambiar ruta");
    changeRouteButtonText.setCharacterSize(18);
    changeRouteButtonText.setFillColor(sf::Color::White);

    coseviButton.setSize(sf::Vector2f(100.f, 40.f));
    coseviButton.setFillColor(sf::Color::Black);
    coseviButton.setOutlineColor(sf::Color::White);
    coseviButton.setOutlineThickness(2.f);

    coseviButtonText.setFont(font);
    coseviButtonText.setString("COSEVI");
    coseviButtonText.setCharacterSize(18);
    coseviButtonText.setFillColor(sf::Color::White);

    newTripButton.setSize(sf::Vector2f(150.f, 40.f));
    newTripButton.setFillColor(sf::Color::Black);
    newTripButton.setOutlineColor(sf::Color::White);
    newTripButton.setOutlineThickness(2.f);

    newTripButtonText.setFont(font);
    newTripButtonText.setString("Nuevo Viaje");
    newTripButtonText.setCharacterSize(18);
    newTripButtonText.setFillColor(sf::Color::White);

    newTripButton.setPosition(10.f, 300.f); 
    centerTextInButton(newTripButtonText, newTripButton);

    initializeComboBox(font);
    resizeUI(window);
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
    clearButton.setPosition(20.f, 300.f);
    centerTextInButton(clearButtonText, clearButton);
    startButton.setPosition(window.getSize().x - 120.f, 20.f);
    centerTextInButton(startButtonText, startButton);
    changeRouteButton.setPosition(window.getSize().x - 120.f, 80.f);
    centerTextInButton(changeRouteButtonText, changeRouteButton);
    newTripButton.setPosition(window.getSize().x - 120.f, 140.f);
    centerTextInButton(newTripButtonText, newTripButton);
    dijkstraCheckBox.setPosition(20.f, 20.f);
    floydCheckBox.setPosition(20.f, 60.f);
    dijkstraText.setPosition(50.f, 20.f);
    floydText.setPosition(50.f, 60.f);
    trafficComboBox.setPosition(20.f, 120.f);
    selectedTrafficText.setPosition(trafficComboBox.getPosition().x + 10.f, trafficComboBox.getPosition().y + 5.f);
    assignAccidentButton.setPosition(20.f, 180.f);
    centerTextInButton(assignAccidentButtonText, assignAccidentButton);
    coseviButton.setPosition(20.f, 240.f);
    centerTextInButton(coseviButtonText, coseviButton);
    toggleWeightsButton.setPosition(window.getSize().x - 120.f, window.getSize().y - 60.f);
    centerTextInButton(toggleWeightsButtonText, toggleWeightsButton);
    toggleStreetsButton.setPosition(window.getSize().x - 120.f, window.getSize().y - 120.f);
    centerTextInButton(toggleStreetsButtonText, toggleStreetsButton);
    totalWeightText.setPosition(20.f, window.getSize().y - 80.f);
    totalCostText.setPosition(20.f, window.getSize().y - 40.f);
}

void UIManager::drawUI(sf::RenderWindow& window) {
    if (showStartButton) {
        window.draw(startButton);
        window.draw(startButtonText);
    }
    window.draw(clearButton);
    window.draw(clearButtonText);
    window.draw(dijkstraCheckBox);
    window.draw(floydCheckBox);
    window.draw(dijkstraText);
    window.draw(floydText);
    window.draw(toggleWeightsButton);
    window.draw(toggleWeightsButtonText);
    window.draw(toggleStreetsButton);
    window.draw(toggleStreetsButtonText);
    window.draw(trafficComboBox);
    window.draw(selectedTrafficText);
    window.draw(assignAccidentButton);
    window.draw(assignAccidentButtonText);
    window.draw(coseviButton);
    window.draw(coseviButtonText);

    if (carroEnMovimiento) {
        window.draw(changeRouteButton);
        window.draw(changeRouteButtonText);
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
        oss << "Peso total: " << totalWeight;
        totalWeightText.setString(oss.str());
    }
}

void UIManager::setTotalCost(float totalCost) {
    if (totalCost == 0.0f) {
        totalCostText.setString("");
    }
    else {
        std::ostringstream oss;
        oss << "Total a pagar: " << totalCost;
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
