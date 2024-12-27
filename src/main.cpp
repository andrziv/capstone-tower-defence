#include <SFML/Graphics.hpp>
#include <optional>

#include "enemy/Enemy.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "SFML works!");
    window.setFramerateLimit(144);

    sf::CircleShape shape(50.f);
    shape.setOrigin(sf::Vector2f(50, 50));
    shape.setFillColor(sf::Color::Green);
    sf::VertexArray enemyPath;
    enemyPath.setPrimitiveType(sf::PrimitiveType::LineStrip);


    sf::Vertex vertices[] {
        {{  0.0f,   430.0f}, sf::Color::Red, { 0.0f,  0.0f}},
        {{  800.0f, 430.0f}, sf::Color::Red, { 0.0f, 10.0f}},
        {{800.0f, 200.0f}, sf::Color::Red, {10.0f, 10.0f}},
        {{  540.0f,   200.0f}, sf::Color::Red, { 0.0f,  0.0f}},
        {{540.0f, 800.0f}, sf::Color::Red, {10.0f, 10.0f}},
        {{280.0f,   800.0f}, sf::Color::Red, {10.0f,  0.0f}},
        {{280.0f,   600.0f}, sf::Color::Red, {10.0f,  0.0f}},
        {{1000.0f,   600.0f}, sf::Color::Red, {10.0f,  0.0f}},
        {{1000.0f,   350.0f}, sf::Color::Red, {10.0f,  0.0f}},
        {{1200.0f,   350.0f}, sf::Color::Red, {10.0f,  0.0f}},
        {{1200.0f,   730.0f}, sf::Color::Red, {10.0f,  0.0f}},
        {{730.0f,   730.0f}, sf::Color::Red, {10.0f,  0.0f}},
        {{730.0f,   1080.0f}, sf::Color::Red, {10.0f,  0.0f}}
    };

    for (const auto & vertice : vertices) {
        enemyPath.append(vertice);
    }
    Enemy everyoneIsMyEnemy(enemyPath, 5);

    sf::VertexArray drawnPath;
    drawnPath.setPrimitiveType(sf::PrimitiveType::LineStrip);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                for (int i = 0; i < drawnPath.getVertexCount(); i++) {
                    printf("{{  %ff,   %ff}, sf::Color::Red, { 0.0f,  0.0f}}", drawnPath.operator[](i).position.x, drawnPath.operator[](i).position.y);
                    if (i != drawnPath.getVertexCount() - 1) {
                        printf(",\n");
                    } else {
                        printf("\n");
                    }
                }
            }
            if (event->is<sf::Event::MouseButtonPressed>()) {
                const auto mousePosition = event->getIf<sf::Event::MouseButtonPressed>()->position;
                sf::Vertex mouseVertex{{static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)}, sf::Color::Yellow, { 0.0f,  0.0f}};
                drawnPath.append(mouseVertex);
            }
        }

        everyoneIsMyEnemy.update();
        shape.setPosition(everyoneIsMyEnemy.getPosition().position);

        window.clear();
        window.draw(shape);
        window.draw(enemyPath);
        window.draw(drawnPath);
        window.display();
    }
}
