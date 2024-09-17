#include "UIManager.h"

UIManager::UIManager(sf::RenderWindow& window, sf::Font& font) {
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

    resizeUI(window);
}

void UIManager::resizeUI(sf::RenderWindow& window) {
    clearButton.setPosition(window.getSize().x - 120.f, 20.f);
    startButton.setPosition(window.getSize().x - 120.f, 80.f);
    dijkstraCheckBox.setPosition(20.f, 20.f);
    floydCheckBox.setPosition(20.f, 60.f);

    centerTextInButton(clearButtonText, clearButton);
    centerTextInButton(startButtonText, startButton);

    dijkstraText.setPosition(50.f, 20.f);
    floydText.setPosition(50.f, 60.f);
}

void UIManager::drawUI(sf::RenderWindow& window) {
    window.draw(clearButton);
    window.draw(clearButtonText);
    window.draw(startButton);
    window.draw(startButtonText);
    window.draw(dijkstraCheckBox);
    window.draw(floydCheckBox);
    window.draw(dijkstraText);
    window.draw(floydText);
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

void UIManager::centerTextInButton(sf::Text& text, const sf::RectangleShape& button) {
    sf::FloatRect textRect = text.getLocalBounds();
    sf::FloatRect buttonRect = button.getGlobalBounds();
    text.setPosition(
        buttonRect.left + (buttonRect.width / 2.0f) - (textRect.width / 2.0f),
        buttonRect.top + (buttonRect.height / 2.0f) - (textRect.height / 2.0f) - 5.f
    );
}
