#ifndef TOWER_H
#define TOWER_H
#include <string>
#include <memory>
#include <utility>

#include "../../../helper/UUID.h"
#include "../../hit_texture/rectangle/RectangleHitTexture.h"
#include "SFML/Graphics.hpp"
#include "../projectile/Projectile.h"


class Tower {
    std::string id = get_uuid();
    sf::Vector2f position;
    float range;
    int cost;
    float attackSpeed;
    int damage;
    std::string type;
    std::shared_ptr<sf::CircleShape> rangeIndicator;
    std::shared_ptr<RectangleHitTexture> hitTexture;

    void setHitTexture(const std::shared_ptr<RectangleHitTexture>& hitTexture) {
        this->hitTexture = hitTexture;
    }

    void setRangeIndicator(const std::shared_ptr<sf::CircleShape>& rangeIndicator) {
        this->rangeIndicator = rangeIndicator;
    }

    public:
    virtual ~Tower() = default;

    Tower(const sf::Vector2f& position, const float range, const int towerCost, const float attackSpeed, const int damage, std::string  type)
        : position(position), range(range), cost(towerCost), attackSpeed(attackSpeed), damage(damage), type(std::move(type)) {
        hitTexture = std::make_shared<RectangleHitTexture>(RectangleHitTexture());
        rangeIndicator = std::make_shared<sf::CircleShape>(sf::CircleShape());
        rangeIndicator->setRadius(range);
        rangeIndicator->setPosition(position);
        rangeIndicator->setFillColor(sf::Color(137, 137, 137, 50)); // Semi-transparent
    }

        virtual std::vector<std::shared_ptr<Projectile>> shootProjectile(std::vector<std::shared_ptr<Enemy>>& enemies) {
            return {};
        }

        [[nodiscard]] sf::Vector2f getPosition() const {
            return position;
        }

        void setPosition(const sf::Vector2f& newPosition) {
            position = newPosition;
            hitTexture->setPosition(newPosition);
            rangeIndicator->setPosition(sf::Vector2f(newPosition.x - rangeIndicator->getRadius(), newPosition.y - rangeIndicator->getRadius()));
        }

        [[nodiscard]] float getRange() const {
            return range;
        }

        [[nodiscard]] int getCost() const {
            return cost;
        }

        [[nodiscard]] float getAttackSpeed() const {
            return attackSpeed;
        }

        [[nodiscard]] int getDamage() const {
            return damage;
        }

        [[nodiscard]] std::string getType() const {
            return type;
        }

        [[nodiscard]] std::string getId() const {
            return id;
        }

        void setId(const std::string& newId) {
            id = newId;
        }

        [[nodiscard]] std::shared_ptr<RectangleHitTexture> getHitTexture() const {
            return hitTexture;
        }

        [[nodiscard]] std::shared_ptr<sf::CircleShape> getRangeIndicator() const {
            return rangeIndicator; // Return the range indicator as a drawable
        }

        void validateRangeIndicator() const {
            auto currentColor = rangeIndicator->getFillColor();
            currentColor.r = 137;
            currentColor.g = 137;
            currentColor.b = 137;
            rangeIndicator->setFillColor(currentColor);
        }

        void invalidateRangeIndicator() const {
            auto currentColor = rangeIndicator->getFillColor();
            currentColor.r = 255;
            currentColor.g = 137;
            currentColor.b = 137;
            rangeIndicator->setFillColor(currentColor);
        }

        std::shared_ptr<Tower> deep_copy() {
            const auto copiedHitTexture = std::make_shared<RectangleHitTexture>(*getHitTexture());
            const auto copiedRangeIndicator = std::make_shared<sf::CircleShape>(*getRangeIndicator());
            const auto copiedDisplayEntity = std::make_shared<sf::RectangleShape>(*getHitTexture()->getRectDisplayEntity());
            const auto copiedHitbox = std::make_shared<sf::RectangleShape>(*getHitTexture()->getRectHitbox());
            auto copiedTower = copy();
            copiedHitTexture->setDisplayEntity(copiedDisplayEntity);
            copiedHitTexture->setHitbox(copiedHitbox);
            copiedTower->setHitTexture(copiedHitTexture);
            copiedTower->setRangeIndicator(copiedRangeIndicator);
            copiedTower->setId(get_uuid());
            return copiedTower;
        }

    protected:
        virtual std::shared_ptr<Tower> copy() = 0;
};

#endif //TOWER_H
