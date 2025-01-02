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
    tickSum -= ticklist[tickIndex];  /* subtract value falling off */
    tickSum += newTick;              /* add new value */
    ticklist[tickIndex] = newTick;   /* save new value so it can be subtracted later */
    if(++tickIndex == MAXSAMPLES)    /* inc buffer index */
        tickIndex = 0;

    /* return average */
    return static_cast<int>(tickSum / MAXSAMPLES);
}

int main() {
    GraphicsManager graphicsManager;
    GameManager gameManager;

    const auto drawnPath = std::make_shared<sf::VertexArray>(sf::VertexArray());
    drawnPath->setPrimitiveType(sf::PrimitiveType::LineStrip);

    graphicsManager.addDrawables(gameManager.getDrawables());
    graphicsManager.addPriorityDrawable(drawnPath);

    sf::Font font;
    auto text = std::make_shared<sf::Text>(sf::Text(font));
    // TODO: fix the file address to target whatever is inside bin
    if (font.openFromFile("../../src/resources/fonts/LEMONMILK-Regular.otf")) {
        text->setCharacterSize(24);
        text->setFillColor(sf::Color::Red);
        text->setStyle(sf::Text::Bold);
        text->setPosition(sf::Vector2f(0, 0));
        graphicsManager.addDrawable(text);
    }

    while (graphicsManager.isActive()) {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        // TODO: move these keeb events to another section of the code
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
                    sf::Vertex mouseVertex{{static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)}, sf::Color::Yellow, { 0.0f,  0.0f}};
                    drawnPath->append(mouseVertex);
                } else if (buttonPressed->button == sf::Mouse::Button::Right) {
                    gameManager.shrinkEnemyPath();
                }
            }
        }

        gameManager.update();
        graphicsManager.addDrawables(gameManager.getNewDrawables());
        graphicsManager.removeDrawables(gameManager.getRemovableDrawables());
        graphicsManager.draw();
        gameManager.cleanup();

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        const double fps = 1e9 / static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
        text->setString(std::to_string(calcAverageTick(fps)).substr(0, 3));
    }
}