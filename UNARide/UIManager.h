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
    void setCarroEnMovimiento(bool enMovimiento);
    bool isDijkstraSelected() const;
    void isCarMoving(sf::RenderWindow& window, bool carroEnMovimiento);

    sf::RectangleShape clearButton;
    sf::RectangleShape startButton;
    sf::RectangleShape assignAccidentButton;
    sf::RectangleShape openStreetButton;
    sf::Text assignAccidentButtonText;
    sf::Text openStreetText;
    sf::RectangleShape dijkstraCheckBox;
    sf::RectangleShape floydCheckBox;
    sf::RectangleShape coseviButton;
    sf::Text coseviButtonText;

    sf::RectangleShape toggleWeightsButton;
    sf::Text toggleWeightsButtonText;

    sf::RectangleShape toggleStreetsButton;
    sf::Text toggleStreetsButtonText;

    sf::RectangleShape trafficComboBox;
    sf::Text selectedTrafficText;

    sf::RectangleShape changeRouteButton;
    sf::Text changeRouteButtonText;

    sf::RectangleShape stopTripButton;
    sf::Text stopTripButtonText;
    sf::RectangleShape continueTripButton;
    sf::Text continueTripButtonText;
    bool isTripStopped;

    sf::RectangleShape newTripButton; 
    sf::Text newTripButtonText;      
    bool showStartButton;
    std::vector<std::string> trafficOptions;
    int selectedTrafficIndex = 0;
    bool showNewTripButton;
    void setShowStartButton(bool value);
    bool isShowStartButton() const;
    void setShowNewTrip(bool value);
    bool isShowNewTrip() const;
    bool showAlgorithmOptions = true;   
    bool showStartOption = false;      
    bool showRouteOptions = false;
    void toggleAlgorithmOptions(bool visible);
    void toggleStartOption(bool visible);
    void toggleRouteOptions(bool visible);
    void setTotalTimeCost();
    void setTotalCompleteCost(double totalCost);
    void initializeClockDisplay(sf::Font& font, sf::RenderWindow& window);
    void updateTimer();
    void updateTimeCost();
    void updateTotalCompleteCost(float totalCost);
    void startClock();
    void stopClock();
    void resetClock();
    void updateClock();
    bool getIsClockRunning() const;
    bool isClockRunning;
    float getCostPerSecond() const;
    void setShowCostLabels(bool show);
    void resetForNewTrip();
    void resetCostLabels();
    void showTrafficButtons(bool show);
private:
    sf::Text timeCostText;
    sf::Text totalCompleteCostText;
    float timeCostPerSecond = 2.0f;
    float totalTimeCost = 0.0f;
    float totalCompleteCost = 0.0f;
    sf::Text clearButtonText;
    sf::Text startButtonText;
    sf::Text dijkstraText;
    sf::Text floydText;
    sf::Text totalWeightText;
    sf::Text totalCostText;
    bool showChangeRoute = false;
    bool carroEnMovimiento;
    void centerTextInButton(sf::Text& text, const sf::RectangleShape& button);
    void initializeComboBox(sf::Font& font);
    sf::Text timeElapsedText;
    sf::Clock clock; 
    int elapsedTimeInSeconds; 
    float costPerSecond; 
    bool showCostLabels = false; 
    bool showTrafficOptions;

public:
    void showChangeRouteButton(bool show);

};



#endif
