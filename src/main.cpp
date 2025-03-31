#include <future>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <optional>
#include <thread>

#include "display/DisplayTextManager.h"
#include "helper/Accumulator.h"
#include "GameManager.h"
#include "GraphicsManager.h"
#include "display/StaticGraphicsManager.h"
#include "entity/hit_texture/animated_sprite/AnimatedSprite.h"
#include "helper/visual/FPS.h"
#include "pressure/DecryptJob.h"
#include "pressure/TowerPressureDecrypt.h"

std::chrono::steady_clock::time_point completionStart = std::chrono::steady_clock::now();
Accumulator completionRate;

[[noreturn]] void decryptSpawner() {
    std::list<std::future<std::string>> results;
    int completions = 0;
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if (activeCores > currentOperations && !toDecrypt.empty()) {
            const auto [toHash, pattern] = decryptNext();
            if (!toHash.empty() && !pattern.empty()) {
                std::packaged_task decryptTask(decrypt);
                std::thread task_td(std::move(decryptTask), activeCores, toHash, pattern, std::ref(decryptedPins));
                task_td.detach();
                currentOperations++;
            }
        }
        while (anyCompletedJobs()) {
            consumeDecrypted();
            completions++;
            currentOperations--;
        }
        const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        const auto timeDiff = std::chrono::duration_cast<std::chrono::seconds>(end - completionStart).count();
        if (timeDiff >= 1) {
            printf("Completions: %d, Rate: %f\n", completions, completionRate.getAverageRate());
            std::cout << std::flush;
            completionRate.accumulate(completions);
            completions = 0;
            completionStart = std::chrono::steady_clock::now();
        }
    }
}

void game_core() {
    GraphicsManager graphicsManager;
    const DisplayTextManager displayTextManager;
    const StaticGraphicsManager menuBackgroundManager;
    GameManager gameManager;
    FPS fps;

    const auto drawnPath = std::make_shared<sf::VertexArray>(sf::VertexArray());
    drawnPath->setPrimitiveType(sf::PrimitiveType::LineStrip);

    graphicsManager.addDrawables(gameManager.getAvailTowerDrawables());
    graphicsManager.addDrawables(gameManager.getDrawables());
    graphicsManager.addPriorityDrawable(drawnPath);
    graphicsManager.addLowPriorityDrawables(menuBackgroundManager.getStaticDrawables());
    graphicsManager.addPriorityDrawables(displayTextManager.getTextDrawables());

    std::shared_ptr<Tower> activeTower = nullptr;
    while (graphicsManager.isActive()) {
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
                        } else if (const auto& selectedTower = gameManager.attemptSelectingPlacedTower(mousePosition); selectedTower != nullptr) {
                            if (activeTower != selectedTower) {
                                if (activeTower) {
                                    graphicsManager.removeDrawable(activeTower->getRangeIndicator());
                                }
                                graphicsManager.addPriorityDrawable(selectedTower->getRangeIndicator());
                                activeTower = selectedTower;

                                displayTextManager.setTowerDamageValue(activeTower->getDamage());
                                displayTextManager.setTowerSpeedValue(activeTower->getAttackSpeed());
                                displayTextManager.setTowerType(activeTower->getType());
                                displayTextManager.setCostOption(activeTower->getCost());
                                displayTextManager.setSellOption(GameManager::getSellPrice(selectedTower));
                            }
                        } else {
                            if (activeTower && displayTextManager.isSellButtonClicked(mousePosition)) {
                                displayTextManager.setSellColorChange(sf::Color(184, 134, 11));
                            } else {
                                if (activeTower) {
                                    displayTextManager.removeTowerStats();
                                    graphicsManager.removeDrawable(activeTower->getRangeIndicator());
                                    activeTower = nullptr;
                                }
                            }
                        }
                    } else {
                        graphicsManager.removeDrawable(gameManager.getHoveredTowerDrawable());
                        graphicsManager.removeDrawable(gameManager.getHoveredTower()->getRangeIndicator());
                        gameManager.deselectTower();
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
                    const sf::Vector2i mousePosition = buttonReleased->position;
                    if (gameManager.isTowerAlreadySelected()) {
                        const auto success = gameManager.addTower(gameManager.getHoveredTower());
                        graphicsManager.removeDrawable(gameManager.getHoveredTower()->getRangeIndicator());
                        gameManager.deselectTower();
                        if (!success) {
                            graphicsManager.removeDrawable(gameManager.getHoveredTowerDrawable());
                        }
                    }
                    if (displayTextManager.isSellButtonClicked(mousePosition) && activeTower != nullptr) {
                        // Remove the tower from the map or list
                        graphicsManager.removeDrawable(activeTower->getHitTexture()->getDisplayEntity());
                        graphicsManager.removeDrawable(activeTower->getRangeIndicator());
                        gameManager.sellTower(activeTower);
                        activeTower = nullptr;  // Deselect the tower
                        displayTextManager.removeTowerStats();
                        displayTextManager.setSellColorChange(sf::Color(255,255,0)); //Reset color
                    }
                }
            }
        }

        if (gameManager.isTowerAlreadySelected()) {
            const auto mousePosition = graphicsManager.getMousePosition();
            gameManager.dragSelectedTower(mousePosition);
            if (const auto selectedTower = gameManager.getHoveredTower()) {
                displayTextManager.setTowerDamageValue(selectedTower->getDamage());
                displayTextManager.setTowerSpeedValue(selectedTower->getAttackSpeed());
                displayTextManager.setTowerType(selectedTower->getType());
                displayTextManager.setCostOption(selectedTower->getCost());
                displayTextManager.setSellOption(GameManager::getSellPrice(selectedTower));

            }
        }

        gameManager.update();

        displayTextManager.setLifeCounterValue(gameManager.getPlayerHealth());
        displayTextManager.setWaveCounterValue(gameManager.getCurrentWaveNumber(), gameManager.getMaxWaveNumber());

        displayTextManager.setPlayerBalanceValue(gameManager.getPlayerBalance());

        graphicsManager.addDrawables(gameManager.getNewDrawables());
        graphicsManager.removeDrawables(gameManager.getRemovableDrawables());
        graphicsManager.draw();
        gameManager.cleanup();

        fps.update();
        displayTextManager.setFPSCounterValue(fps.getFPS());

        displayTextManager.setRemainingPressureValue(static_cast<int>(toDecrypt.size()) + currentOperations);
        displayTextManager.setActivePressureJobsValue(currentOperations);
        displayTextManager.setPressureCompletionRateValue(completionRate.getAverageRate());
        displayTextManager.setPressureProductionRateValue(additionRate.getAverageRate());
    }
}

int main() {
    // std::thread thread(decryptSpawner);
    // thread.detach();
    setActiveCoresTo(0);
    game_core();
}
