#ifndef DISPLAYTEXTMANAGER_H
#define DISPLAYTEXTMANAGER_H
#include <memory>

#include "DisplayConsts.h"
#include "helper/Digits.h"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Vector2.hpp"


class DisplayTextManager {
    sf::Font font;
    const std::shared_ptr<sf::Text> fpsCounter = std::make_shared<sf::Text>(sf::Text(font));
    const std::shared_ptr<sf::Text> lifeCounter = std::make_shared<sf::Text>(sf::Text(font));
    const std::shared_ptr<sf::Text> waveCounter = std::make_shared<sf::Text>(sf::Text(font));
    const std::shared_ptr<sf::Text> pressureRemaining = std::make_shared<sf::Text>(sf::Text(font));
    const std::shared_ptr<sf::Text> jobCounter = std::make_shared<sf::Text>(sf::Text(font));
    const std::shared_ptr<sf::Text> pressureAdditionRate = std::make_shared<sf::Text>(sf::Text(font));
    const std::shared_ptr<sf::Text> pressureCompletionRate = std::make_shared<sf::Text>(sf::Text(font));
    const std::shared_ptr<sf::Text> balanceCounter = std::make_shared<sf::Text>(sf::Text(font));

public:
    DisplayTextManager() {
        if (font.openFromFile("../../src/resources/fonts/LEMONMILK-Regular.otf")) {
            fpsCounter->setCharacterSize(24);
            fpsCounter->setFillColor(sf::Color::Red);
            fpsCounter->setStyle(sf::Text::Bold);
            fpsCounter->setPosition(sf::Vector2f(FPS_START_RIGHT, TEXT_START_Y));

            lifeCounter->setCharacterSize(24);
            lifeCounter->setFillColor(sf::Color::Red);
            lifeCounter->setStyle(sf::Text::Bold);
            lifeCounter->setPosition(sf::Vector2f(LIFE_START_RIGHT, TEXT_START_Y));

            pressureRemaining->setCharacterSize(24);
            pressureRemaining->setFillColor(sf::Color(150, 80, 80));
            pressureRemaining->setStyle(sf::Text::Bold);
            pressureRemaining->setPosition(sf::Vector2f(PRESSURE_START_RIGHT, TEXT_START_Y));

            jobCounter->setCharacterSize(24);
            jobCounter->setFillColor(sf::Color(80, 150, 150));
            jobCounter->setStyle(sf::Text::Bold);
            jobCounter->setPosition(sf::Vector2f(PRESSURE_START_RIGHT, PRESSURE_GAP_Y * 1));

            pressureCompletionRate->setCharacterSize(24);
            pressureCompletionRate->setFillColor(sf::Color(17, 124, 19));
            pressureCompletionRate->setStyle(sf::Text::Bold);
            pressureCompletionRate->setPosition(sf::Vector2f(PRESSURE_START_RIGHT, PRESSURE_GAP_Y * 2));

            pressureAdditionRate->setCharacterSize(24);
            pressureAdditionRate->setFillColor(sf::Color(255, 99, 71));
            pressureAdditionRate->setStyle(sf::Text::Bold);
            pressureAdditionRate->setPosition(sf::Vector2f(PRESSURE_START_RIGHT, PRESSURE_GAP_Y * 3));

            waveCounter->setCharacterSize(24);
            waveCounter->setFillColor(sf::Color::Red);
            waveCounter->setStyle(sf::Text::Bold);
            waveCounter->setPosition(sf::Vector2f(WAVE_START_RIGHT, TEXT_START_Y));

            balanceCounter->setCharacterSize(24);
            balanceCounter->setFillColor(sf::Color::Yellow);
            balanceCounter->setStyle(sf::Text::Bold);
            balanceCounter->setPosition(sf::Vector2f(BALANCE_START_RIGHT, TEXT_START_Y));
        }
    }

    void setLifeCounterValue(const int newHealth) const {
        lifeCounter->setString("Lives: " + std::to_string(newHealth));
    }

    void setWaveCounterValue(const int newWave, const int maxWaves) const {
        waveCounter->setString(std::to_string(newWave) + " / " + std::to_string(maxWaves));
    }

    void setPlayerBalanceValue(const int newBalance) const {
        balanceCounter->setString("Balance: " + std::to_string(newBalance));
    }

    void setFPSCounterValue(const unsigned int newFPS) const {
        fpsCounter->setString(std::to_string(newFPS));
    }

    void setRemainingPressureValue(const int newRemaining) const {
        pressureRemaining->setString("Remaining Pressure Jobs: " + std::to_string(newRemaining));
    }

    void setActivePressureJobsValue(const int newActive) const {
        jobCounter->setString("Active Pressure Jobs: " + std::to_string(newActive));
    }

    void setPressureCompletionRateValue(const double newRate) const {
        pressureCompletionRate->setString("Completion Rate: " + std::to_string(newRate).substr(0, countDigit(static_cast<int>(newRate)) + 3));
    }

    void setPressureProductionRateValue(const double newRate) const {
        pressureAdditionRate->setString("Production Rate: " + std::to_string(newRate).substr(0, countDigit(static_cast<int>(newRate)) + 3));
    }

    std::vector<std::shared_ptr<sf::Drawable>> getTextDrawables() {
        return {
            fpsCounter,
            lifeCounter,
            pressureRemaining,
            jobCounter,
            pressureCompletionRate,
            pressureAdditionRate,
            waveCounter,
            balanceCounter
        };
    }
};


#endif //DISPLAYTEXTMANAGER_H
