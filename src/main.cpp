#include <SFML/Graphics.hpp>
#include <optional>
#include <thread>

#include "GameManager.h"
#include "GraphicsManager.h"

#define MAXSAMPLES 100

int tickIndex = 0;
double tickSum = 0;
double ticklist[MAXSAMPLES];

int calcAverageTick(const double newTick) {
    tickSum -= ticklist[tickIndex];  
    tickSum += newTick;              
    ticklist[tickIndex] = newTick;   
    if (++tickIndex == MAXSAMPLES)    
        tickIndex = 0;

    return static_cast<int>(tickSum / MAXSAMPLES);
}

int main() {
    GraphicsManager graphicsManager;
    GameManager gameManager;

    const auto drawnPath = std::make_shared<sf::VertexArray>(sf::VertexArray());
    drawnPath->setPrimitiveType(sf::PrimitiveType::LineStrip);

    graphicsManager.addDrawables(gameManager.getAvailTowerDrawables());
    graphicsManager.addDrawables(gameManager.getDrawables());
    graphicsManager.addPriorityDrawable(drawnPath);

    sf::Font font;
    const auto fpsCounter = std::make_shared<sf::Text>(sf::Text(font));
    const auto waveCounter = std::make_shared<sf::Text>(sf::Text(font));
    const auto lifeCounter = std::make_shared<sf::Text>(sf::Text(font));
    const auto balanceCounter = std::make_shared<sf::Text>(sf::Text(font));  
    if (font.openFromFile("../../src/resources/fonts/LEMONMILK-Regular.otf")) {
        fpsCounter->setCharacterSize(24);
        fpsCounter->setFillColor(sf::Color::Red);
        fpsCounter->setStyle(sf::Text::Bold);
        fpsCounter->setPosition(sf::Vector2f(0, 0));
        graphicsManager.addDrawable(fpsCounter);

        waveCounter->setCharacterSize(24);
        waveCounter->setFillColor(sf::Color::Red);
        waveCounter->setStyle(sf::Text::Bold);
        waveCounter->setPosition(sf::Vector2f(1450, 0));
        graphicsManager.addDrawable(waveCounter);

        lifeCounter->setCharacterSize(24);
        lifeCounter->setFillColor(sf::Color::Red);
        lifeCounter->setStyle(sf::Text::Bold);
        lifeCounter->setPosition(sf::Vector2f(150, 0));
        graphicsManager.addDrawable(lifeCounter);

        balanceCounter->setCharacterSize(24);  
        balanceCounter->setFillColor(sf::Color::Yellow);  
        balanceCounter->setStyle(sf::Text::Bold);
        balanceCounter->setPosition(sf::Vector2f(1550, 50));  
        graphicsManager.addDrawable(balanceCounter);  
    }

    while (graphicsManager.isActive()) {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
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
                        }
                    } else {
                        graphicsManager.removeDrawable(gameManager.getHoveredTowerDrawable());
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

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        const double fps = 1e9 / static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
        fpsCounter->setString(std::to_string(calcAverageTick(fps)).substr(0, 3));
    }
}
