#ifndef TOWER_H
#define TOWER_H
#include <string>
#include <memory>

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
    sf::CircleShape rangeIndicator;
    std::shared_ptr<RectangleHitTexture> hitTexture;

    public:
    virtual ~Tower() = default;

        Tower(const sf::Vector2f& position, const float range, const int towerCost, const float attackSpeed)
            : position(position), range(range), cost(towerCost), attackSpeed(attackSpeed) {
            hitTexture = std::make_shared<RectangleHitTexture>(RectangleHitTexture());
            rangeIndicator.setRadius(range);
            rangeIndicator.setOrigin(sf::Vector2f(range, range));
            rangeIndicator.setPosition(position);
            rangeIndicator.setFillColor(sf::Color(255, 255, 255, 50)); // Semi-transparent
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
            rangeIndicator.setPosition(newPosition);
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

        std::string getId() const {
            return id;
        }

        void setId(const std::string& newId) {
            id = newId;
        }

        [[nodiscard]] std::shared_ptr<RectangleHitTexture> getHitTexture() const {
            return hitTexture;
        }

        void setHitTexture(const std::shared_ptr<RectangleHitTexture>& hitTexture) {
            this->hitTexture = hitTexture;
        }

        [[nodiscard]] std::shared_ptr<sf::CircleShape> getRangeIndicator() const {
            return std::make_shared<sf::CircleShape>(rangeIndicator); // Return the range indicator as a drawable
        }

        std::shared_ptr<Tower> deep_copy() {
            const auto copiedHitTexture = std::make_shared<RectangleHitTexture>(*getHitTexture());
            const auto copiedDisplayEntity = std::make_shared<sf::RectangleShape>(*getHitTexture()->getRectDisplayEntity());
            const auto copiedHitbox = std::make_shared<sf::RectangleShape>(*getHitTexture()->getRectHitbox());
            auto copiedTower = copy(); // make a copy of the available tower
            copiedHitTexture->setDisplayEntity(copiedDisplayEntity);
            copiedHitTexture->setHitbox(copiedHitbox);
            copiedTower->setHitTexture(copiedHitTexture);
            return copiedTower;
        }

    protected:
        virtual std::shared_ptr<Tower> copy() = 0;
};

#endif //TOWER_H
