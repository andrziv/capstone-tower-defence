#ifndef STATICGRAPHICSMANAGER_H
#define STATICGRAPHICSMANAGER_H
#include <memory>

#include "DisplayConsts.h"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"


class StaticGraphicsManager {
    std::shared_ptr<sf::RectangleShape> selectionMenu;

public:
    StaticGraphicsManager() {
        this->selectionMenu = std::make_shared<sf::RectangleShape>(sf::RectangleShape());
        selectionMenu->setPosition(sf::Vector2f(SEL_MENU_START_X, 0));
        selectionMenu->setSize(sf::Vector2f(DISPLAY_MAX_X, DISPLAY_MAX_Y));
        selectionMenu->setFillColor(sf::Color(33, 33, 33, 125));
    }

    [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getStaticDrawables() const {
        return {
            selectionMenu
        };
    }
};



#endif //STATICGRAPHICSMANAGER_H
