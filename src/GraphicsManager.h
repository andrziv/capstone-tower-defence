#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H
#include <SFML/Graphics.hpp>
#include <list>

class GraphicsManager {
    sf::RenderWindow *window;
    std::list<sf::Drawable*> drawables;

public:
    GraphicsManager() {
        this->window = new sf::RenderWindow(sf::VideoMode({1920, 1080}), "SFML works!");
        this->window->setFramerateLimit(144);
    }

    void addDrawable(sf::Drawable* drawable) {
        drawables.push_back(drawable);
    }

    void addPriorityDrawable(sf::Drawable* drawable) {
        drawables.push_front(drawable);
    }

    void removeDrawable(sf::Drawable* drawable) {
        drawables.remove(drawable);
    }

    bool isActive() const {
        return window->isOpen();
    }

    void deactivate() const {
        window->close();
    }

    std::optional<sf::Event> pollEvent() const {
        return window->pollEvent();
    }

    void draw() const {
        window->clear();
        for (const auto drawable : drawables) {
            window->draw(*drawable);
        }
        window->display();
    }
};



#endif //GRAPHICSMANAGER_H
