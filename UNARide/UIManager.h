#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>

class UIManager {
public:
    UIManager(sf::RenderWindow& window, sf::Font& font);
    void resizeUI(sf::RenderWindow& window);
    void drawUI(sf::RenderWindow& window);
    void setAlgorithmSelected(bool isDijkstra);

    sf::RectangleShape clearButton;
    sf::RectangleShape startButton;
    sf::RectangleShape dijkstraCheckBox;
    sf::RectangleShape floydCheckBox;

private:
    sf::Text clearButtonText;
    sf::Text startButtonText;
    sf::Text dijkstraText;
    sf::Text floydText;

    void centerTextInButton(sf::Text& text, const sf::RectangleShape& button);
};

#endif