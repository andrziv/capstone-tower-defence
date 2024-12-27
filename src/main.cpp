#include <SFML/Graphics.hpp>
#include <optional>

#include "enemy/Enemy.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "SFML works!");
    window.setFramerateLimit(144);

    sf::CircleShape shape(50.f);
    shape.setOrigin(sf::Vector2f(50, 50));
    shape.setFillColor(sf::Color::Green);
    sf::VertexArray drawable;

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

    drawable.setPrimitiveType(sf::PrimitiveType::LineStrip);
    for (const auto & vertice : vertices) {
        drawable.append(vertice);
    }

    Enemy everyoneIsMyEnemy(drawable, 1);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        everyoneIsMyEnemy.update();
        shape.setPosition(everyoneIsMyEnemy.getPosition().position);

        window.clear();
        window.draw(shape);
        window.draw(drawable);
        window.display();
    }
}
