#ifndef RECTANGLEHITTEXTURE_H
#define RECTANGLEHITTEXTURE_H
#include "../HitTexture.h"
#include "SFML/Graphics/RectangleShape.hpp"


class RectangleHitTexture final : HitTexture{
    std::shared_ptr<sf::RectangleShape> displayEntity;
    std::shared_ptr<sf::RectangleShape> hitbox;

    public:
        RectangleHitTexture() {
            this->displayEntity = std::make_shared<sf::RectangleShape>(sf::RectangleShape());
            this->hitbox = std::make_shared<sf::RectangleShape>(sf::RectangleShape());
        }

        [[nodiscard]] std::shared_ptr<sf::Shape> getHitbox() const override {
            return hitbox;
        }

        [[nodiscard]] std::shared_ptr<sf::RectangleShape> getRectHitbox() const {
            return hitbox;
        }

        void setHitboxColor(const sf::Color color) const {
            hitbox->setFillColor(color);
        }

        [[nodiscard]] std::shared_ptr<sf::Shape> getDisplayEntity() const override {
            return displayEntity;
        }

        [[nodiscard]] std::shared_ptr<sf::RectangleShape> getRectDisplayEntity() const {
            return displayEntity;
        }

        void setDisplayEntityColor(const sf::Color color) const {
            displayEntity->setFillColor(color);
        }

        void setPosition(const float x, const float y) const override {
            displayEntity->setPosition(sf::Vector2f(
                x - (displayEntity->getGeometricCenter().x - displayEntity->getPosition().x),
                y - (displayEntity->getGeometricCenter().y - displayEntity->getPosition().y)));
            hitbox->setPosition(sf::Vector2f(
                x + ((displayEntity->getGeometricCenter().x - displayEntity->getPosition().x) - (hitbox->getGeometricCenter().x - hitbox->getPosition().x)),
                y + ((displayEntity->getGeometricCenter().y - displayEntity->getPosition().y) - (hitbox->getGeometricCenter().y - hitbox->getPosition().y))));
        }

        void setPosition(const sf::Vector2f position) const override {
            setPosition(position.x, position.y);
        }
};



#endif //RECTANGLEHITTEXTURE_H
