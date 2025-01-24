#ifndef TOWER_H
#define TOWER_H
#include <string>
#include <memory>

#include "../../../helper/UUID.h"
#include "SFML/Graphics/Vertex.hpp"
#include "SFML/Graphics.hpp"
#include "../projectile/Projectile.h"


class Tower {
    std::string id = get_uuid();
    sf::Vector2f position;
    float range;
    int attackPower;
    float attackSpeed;
    sf::CircleShape rangeIndicator;

    public:
    virtual ~Tower() = default;

    Tower(const sf::Vector2f& position, float range, int attackPower, float attackSpeed)
            : position(position), range(range), attackPower(attackPower), attackSpeed(attackSpeed) {
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
            rangeIndicator.setPosition(newPosition);
        }

        [[nodiscard]] float getRange() const {
            return range;
        }

        [[nodiscard]] int getAttackPower() const {
            return attackPower;
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

        // New method to get the tower's display entity (for rendering)
        [[nodiscard]] std::shared_ptr<sf::Drawable> getDisplayEntity() const {
            return std::make_shared<sf::CircleShape>(rangeIndicator); // Return the range indicator as a drawable
        }
};

#endif //TOWER_H
