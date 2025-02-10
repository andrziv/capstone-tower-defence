#include <iostream>
#include <SFML/Graphics.hpp>
#include <optional>
#include <thread>

#include "Accumulator.h"
#include "FPS.h"
#include "GameManager.h"
#include "GraphicsManager.h"
#include "TowerPressureDecrpt.h"
#include "helper/Digits.h"

std::chrono::steady_clock::time_point completionStart = std::chrono::steady_clock::now();
Accumulator additionRate;
Accumulator completionRate;

[[noreturn]] void decryptSpawner() {
    int completions = 0;
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if (activeCores > currentOperations && !toDecrypt.empty()) {
            std::thread thread(decryptNext);
            thread.detach();
        }
        const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        const auto timeDiff = std::chrono::duration_cast<std::chrono::seconds>(end - completionStart).count();
        if (timeDiff >= 1) {
            while (anyCompletedJobs()) {
                consumeDecrypted();
                completions++;
            }
            printf("Completions: %d, Rate: %f\n", completions, completionRate.getAverageRate());
            std::cout << std::flush;
            completionRate.accumulate(completions);
            completions = 0;
            completionStart = std::chrono::steady_clock::now();
        }

        //printf("Pressure Jobs in Queue: %lu, Active Threads: %d\n", toDecrypt.size(), currentOperations);
        //std::cout << std::flush;
    }
}

void game_core() {
    std::thread thread(decryptSpawner);
    thread.detach();

    GraphicsManager graphicsManager;
    GameManager gameManager;
    FPS fps;

    const auto drawnPath = std::make_shared<sf::VertexArray>(sf::VertexArray());
    drawnPath->setPrimitiveType(sf::PrimitiveType::LineStrip);

    graphicsManager.addDrawables(gameManager.getAvailTowerDrawables());
    graphicsManager.addDrawables(gameManager.getDrawables());
    graphicsManager.addPriorityDrawable(drawnPath);

    sf::Font font;
    const auto fpsCounter = std::make_shared<sf::Text>(sf::Text(font));
    const auto lifeCounter = std::make_shared<sf::Text>(sf::Text(font));
    const auto waveCounter = std::make_shared<sf::Text>(sf::Text(font));
    const auto pressureAdditionRate = std::make_shared<sf::Text>(sf::Text(font));
    const auto pressureCompletionRate = std::make_shared<sf::Text>(sf::Text(font));
    const auto balanceCounter = std::make_shared<sf::Text>(sf::Text(font));
    if (font.openFromFile("../../src/resources/fonts/LEMONMILK-Regular.otf")) {
        fpsCounter->setCharacterSize(24);
        fpsCounter->setFillColor(sf::Color::Red);
        fpsCounter->setStyle(sf::Text::Bold);
        fpsCounter->setPosition(sf::Vector2f(0, 0));
        graphicsManager.addDrawable(fpsCounter);

        lifeCounter->setCharacterSize(24);
        lifeCounter->setFillColor(sf::Color::Red);
        lifeCounter->setStyle(sf::Text::Bold);
        lifeCounter->setPosition(sf::Vector2f(150, 0));
        graphicsManager.addDrawable(lifeCounter);

        pressureCompletionRate->setCharacterSize(24);
        pressureCompletionRate->setFillColor(sf::Color(17,124,19));
        pressureCompletionRate->setStyle(sf::Text::Bold);
        pressureCompletionRate->setPosition(sf::Vector2f(500, 0));
        graphicsManager.addDrawable(pressureCompletionRate);

        pressureAdditionRate->setCharacterSize(24);
        pressureAdditionRate->setFillColor(sf::Color(255,99,71));
        pressureAdditionRate->setStyle(sf::Text::Bold);
        pressureAdditionRate->setPosition(sf::Vector2f(575, 0));
        graphicsManager.addDrawable(pressureAdditionRate);

        waveCounter->setCharacterSize(24);
        waveCounter->setFillColor(sf::Color::Red);
        waveCounter->setStyle(sf::Text::Bold);
        waveCounter->setPosition(sf::Vector2f(1450, 0));
        graphicsManager.addDrawable(waveCounter);

        balanceCounter->setCharacterSize(24);
        balanceCounter->setFillColor(sf::Color::Yellow);
        balanceCounter->setStyle(sf::Text::Bold);
        balanceCounter->setPosition(sf::Vector2f(1550, 50));
        graphicsManager.addDrawable(balanceCounter);
    }
    setActiveCoresTo(3);
    int dummy;
    while (graphicsManager.isActive()) {
        dummy++;
        while (const std::optional event = graphicsManager.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                graphicsManager.deactivate();
                for (int i = 0; i < drawnPath->getVertexCount(); i++) {
                    printf("{{  %ff,   %ff}, sf::Color::Red, { 0.0f,  0.0f}}", drawnPath->operator[](i).position.x, drawnPath->operator[](i).position.y);
                    if (i != drawnPath->getVertexCount() - 1) {
                        printf(",\n");
                    } else {
                        printf("\n");
                    }
                }
            }

            if (event->is<sf::Event::MouseButtonPressed>()) {
                if (const auto buttonPressed = event->getIf<sf::Event::MouseButtonPressed>(); buttonPressed->button == sf::Mouse::Button::Left) {
                    const auto mousePosition = buttonPressed->position;
                    if (!gameManager.isTowerAlreadySelected()) {
                        if (gameManager.attemptSelectingTower(mousePosition)) {
                            graphicsManager.addPriorityDrawable(gameManager.getHoveredTowerDrawable());
                            graphicsManager.addPriorityDrawable(gameManager.getHoveredTower()->getRangeIndicator());
                        }
                    } else {
                        graphicsManager.removeDrawable(gameManager.getHoveredTowerDrawable());
                        graphicsManager.removeDrawable(gameManager.getHoveredTower()->getRangeIndicator());
                    }
                } else if (buttonPressed->button == sf::Mouse::Button::Right) {
                    gameManager.shrinkEnemyPath();
                }
            }

            if (event->is<sf::Event::KeyPressed>()) {
                if (const auto buttonPressed = event->getIf<sf::Event::KeyPressed>(); buttonPressed->code == sf::Keyboard::Key::Space) {
                    graphicsManager.setFramerateLimit(240);
                } else {
                    graphicsManager.setFramerateLimit(60);
                }
            }

            if (event->is<sf::Event::MouseButtonReleased>()) {
                if (const auto buttonReleased = event->getIf<sf::Event::MouseButtonReleased>(); buttonReleased->button == sf::Mouse::Button::Left) {
                    if (gameManager.isTowerAlreadySelected()) {
                        const auto success = gameManager.addTower(gameManager.getHoveredTower());
                        graphicsManager.removeDrawable(gameManager.getHoveredTower()->getRangeIndicator());
                        gameManager.deselectTower();
                        if (!success) {
                            graphicsManager.removeDrawable(gameManager.getHoveredTowerDrawable());
                        }
                    }
                }
            }
        }

        if (gameManager.isTowerAlreadySelected()) {
            const auto mousePosition = graphicsManager.getMousePosition();
            gameManager.dragSelectedTower(mousePosition);
        }

        gameManager.update();

        lifeCounter->setString("Lives: " + std::to_string(gameManager.getPlayerHealth()));
        waveCounter->setString(std::to_string(gameManager.getCurrentWaveNumber()) + " / " + std::to_string(gameManager.getMaxWaveNumber()));

        balanceCounter->setString("Balance: " + std::to_string(gameManager.getPlayerBalance()));

        graphicsManager.addDrawables(gameManager.getNewDrawables());
        graphicsManager.removeDrawables(gameManager.getRemovableDrawables());
        graphicsManager.draw();
        gameManager.cleanup();

        fps.update();
        fpsCounter->setString(std::to_string(fps.getFPS()));
        int digits = countDigit(static_cast<int>(completionRate.getAverageRate()));
        pressureCompletionRate->setString(std::to_string(completionRate.getAverageRate()).substr(0, digits + 3));
        pressureAdditionRate->setString(std::to_string(additionRate.getAverageRate()).substr(0, digits + 3));
    }
}

int main() {
    game_core();
}
