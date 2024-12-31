#ifndef HITTEXTURE_H
#define HITTEXTURE_H

#include <memory>

#include "SFML/Graphics/CircleShape.hpp"

class HitTexture {
    std::shared_ptr<sf::CircleShape> displayEntity;
    std::shared_ptr<sf::CircleShape> hitbox;

    public:
        HitTexture() {
            this->displayEntity = std::make_shared<sf::CircleShape>(sf::CircleShape());
            this->hitbox = std::make_shared<sf::CircleShape>(sf::CircleShape());
        }

        [[nodiscard]] sf::CircleShape getHitbox() const {
            return *hitbox;
        }

        void setHitboxColor(const sf::Color color) const {
            hitbox->setFillColor(color);
        }

        void setHitboxRadius(const float radius) const {
            hitbox->setRadius(radius);
        }

        [[nodiscard]] std::shared_ptr<sf::CircleShape> getDisplayEntity() const {
            return displayEntity;
        }

        void setPosition(const float x, const float y) const {
            displayEntity->setPosition(sf::Vector2f(x - displayEntity->getRadius(), y - displayEntity->getRadius()));
            hitbox->setPosition(sf::Vector2f(x + (displayEntity->getRadius() - hitbox->getRadius()), y + (displayEntity->getRadius() - hitbox->getRadius())));
        }

        void setPosition(const sf::Vector2f position) const {
            setPosition(position.x, position.y);
        }
};



#endif //HITTEXTURE_H
