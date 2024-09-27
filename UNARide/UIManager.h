#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>

class UIManager {
public:
    UIManager(sf::RenderWindow& window, sf::Font& font);
    void resizeUI(sf::RenderWindow& window);
    void drawUI(sf::RenderWindow& window);
    void setAlgorithmSelected(bool isDijkstra);
    void setTotalWeight(float totalWeight);
    void setTotalCost(float totalCost);
    void resetAlgorithmSelected();
    int getTrafficMultiplier() const;

    sf::RectangleShape clearButton;
    sf::RectangleShape startButton;
    sf::RectangleShape dijkstraCheckBox;
    sf::RectangleShape floydCheckBox;

    sf::RectangleShape toggleWeightsButton;
    sf::Text toggleWeightsButtonText;

    sf::RectangleShape toggleStreetsButton;
    sf::Text toggleStreetsButtonText;

    sf::RectangleShape trafficComboBox;
    sf::Text selectedTrafficText;

    std::vector<std::string> trafficOptions;
    int selectedTrafficIndex = 0;

private:
    sf::Text clearButtonText;
    sf::Text startButtonText;
    sf::Text dijkstraText;
    sf::Text floydText;
    sf::Text totalWeightText;
    sf::Text totalCostText;

    void centerTextInButton(sf::Text& text, const sf::RectangleShape& button);
    void initializeComboBox(sf::Font& font);
};

#endif
