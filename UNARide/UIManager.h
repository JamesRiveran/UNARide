#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>

class UIManager {
public:
    UIManager(sf::RenderWindow& window, sf::Font& font);
    void resizeUI(sf::RenderWindow& window);
    void drawUI(sf::RenderWindow& window);
    void setAlgorithmSelected(bool isDijkstra);
    void setTotalWeight(float totalWeight);
    void setTotalCost(float totalCost);

    sf::RectangleShape clearButton;
    sf::RectangleShape startButton;
    sf::RectangleShape dijkstraCheckBox;
    sf::RectangleShape floydCheckBox;

    sf::RectangleShape toggleWeightsButton;
    sf::Text toggleWeightsButtonText;

    sf::RectangleShape toggleStreetsButton;
    sf::Text toggleStreetsButtonText;

private:
    sf::Text clearButtonText;
    sf::Text startButtonText;
    sf::Text dijkstraText;
    sf::Text floydText;
    sf::Text totalWeightText;
    sf::Text totalCostText;

    void centerTextInButton(sf::Text& text, const sf::RectangleShape& button);
};

#endif
