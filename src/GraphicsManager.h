#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H
#include <SFML/Graphics.hpp>
#include <list>

class GraphicsManager {
    std::unique_ptr<sf::RenderWindow> window;
    std::list<std::shared_ptr<sf::Drawable>> drawables;

public:
    GraphicsManager() {
        this->window = std::make_unique<sf::RenderWindow>(sf::RenderWindow(sf::VideoMode({1920, 1080}), "SFML works!"));
        this->window->setFramerateLimit(60);
    }

    void addDrawable(const std::shared_ptr<sf::Drawable>& drawable) {
        drawables.push_back(drawable);
    }

    void addDrawables(const std::vector<std::shared_ptr<sf::Drawable>>& newDrawables) {
        for (const auto& drawable : newDrawables) {
            drawables.push_back(drawable);
        }
    }

    void addPriorityDrawable(const std::shared_ptr<sf::Drawable>& drawable) {
        drawables.push_front(drawable);
    }

    void addPriorityDrawables(const std::vector<std::shared_ptr<sf::Drawable>>& newDrawables) {
        for (const auto& drawable : newDrawables) {
            drawables.push_back(drawable);
        }
    }

    void removeDrawable(const std::shared_ptr<sf::Drawable>& toRemove) {
        drawables.remove(toRemove);
    }

    void removeDrawables(const std::vector<std::shared_ptr<sf::Drawable>>& toRemove) {
        for (const std::shared_ptr<sf::Drawable>& drawable : toRemove) {
            drawables.remove(drawable);
        }
    }

    [[nodiscard]] bool isActive() const {
        return window->isOpen();
    }

    void deactivate() const {
        window->close();
    }

    [[nodiscard]] std::optional<sf::Event> pollEvent() const {
        return window->pollEvent();
    }

    void draw() const {
        window->clear();
        for (const auto& drawable : drawables) {
            window->draw(*drawable);
        }
        window->display();
    }
};



#endif //GRAPHICSMANAGER_H
