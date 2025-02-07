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

        void setHitboxHeight(const float height) const {
            auto size = hitbox->getSize();
            size.y = height;
            hitbox->setSize(size);
        }

        void setHitboxWidth(const float width) const {
            auto size = hitbox->getSize();
            size.x = width;
            hitbox->setSize(size);
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

        void setDisplayEntityHeight(const float height) const {
            auto size = displayEntity->getSize();
            size.y = height;
            displayEntity->setSize(size);
        }

        void setDisplayEntityWidth(const float width) const {
            auto size = displayEntity->getSize();
            size.x = width;
            displayEntity->setSize(size);
        }

        void setPosition(const float x, const float y) const override {
            displayEntity->setPosition(sf::Vector2f(x, y));
            hitbox->setPosition(sf::Vector2f(
                x + (displayEntity->getGeometricCenter().x - hitbox->getGeometricCenter().x),
                y + (displayEntity->getGeometricCenter().y - hitbox->getGeometricCenter().y)));
        }

        void setPosition(const sf::Vector2f position) const override {
            setPosition(position.x, position.y);
        }

        void setHitbox(const std::shared_ptr<sf::RectangleShape>& hitbox) {
            this->hitbox = hitbox;
        }

        void setDisplayEntity(const std::shared_ptr<sf::RectangleShape>& displayEntity) {
            this->displayEntity = displayEntity;
        }
};



#endif //RECTANGLEHITTEXTURE_H
