#ifndef HITTEXTURE_H
#define HITTEXTURE_H

#include "SFML/Graphics/CircleShape.hpp"

class HitTexture {
    sf::CircleShape *displayEntity;
    sf::CircleShape *hitbox;

    public:
        HitTexture() {
            this->displayEntity = new sf::CircleShape();
            this->hitbox = new sf::CircleShape();
        }

        HitTexture(sf::CircleShape *displayEntity, sf::CircleShape *hitbox) {
            this->displayEntity = displayEntity;
            this->hitbox = hitbox;
            this->hitbox->setOrigin(this->displayEntity->getGeometricCenter());
        }

        [[nodiscard]] sf::CircleShape *getHitbox() const {
            return hitbox;
        }

        sf::CircleShape *getDisplayEntity() const {
            return displayEntity;
        }

        void setPosition(const int x, const int y) const {
            displayEntity->setPosition(sf::Vector2f(x - displayEntity->getRadius(), y - displayEntity->getRadius()));
            hitbox->setPosition(sf::Vector2f(x + (displayEntity->getRadius() - hitbox->getRadius()), y + (displayEntity->getRadius() - hitbox->getRadius())));
        }

        void setPosition(const sf::Vector2f position) const {
            setPosition(position.x, position.y);
        }
};



#endif //HITTEXTURE_H
