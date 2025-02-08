//
// Created by aki on 2/7/25.
//

#ifndef BASETOWER_H
#define BASETOWER_H
#include "../Tower.h"


class BaseTower : public Tower {
    public:
        explicit BaseTower(const sf::Vector2f& position, const float range, const int towerCost, const float attackSpeed, const sf::Color color)
            : Tower (position, range, towerCost, attackSpeed)  {
            getHitTexture()->setDisplayEntityColor(color);
            getHitTexture()->setDisplayEntityHeight(50);
            getHitTexture()->setDisplayEntityWidth(50);
            getHitTexture()->setHitboxColor(sf::Color::Transparent);
            getHitTexture()->setHitboxHeight(50);
            getHitTexture()->setHitboxWidth(50);
        }

        std::vector<std::shared_ptr<Projectile>> shootProjectile(std::vector<std::shared_ptr<Enemy>>& enemies) override {
            const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            const auto timeDiff = std::chrono::duration_cast<std::chrono::seconds>(end - shootStart).count();
            std::vector<std::shared_ptr<Projectile>> projectiles;
            if (static_cast<float>(timeDiff) >= getAttackSpeed()) {
                auto [aimAngle, enemyFound] = calculateAimAngle(enemies);

                if (enemyFound){
                    for (const auto& projectile : generateProjectiles(aimAngle)) {
                        projectiles.push_back(projectile);
                    }

                    shootStart = std::chrono::steady_clock::now();
                }
            }
            return projectiles;
        }

    protected:
        std::chrono::steady_clock::time_point shootStart = std::chrono::steady_clock::now();

        virtual std::pair<double, bool> calculateAimAngle(std::vector<std::shared_ptr<Enemy>>& enemies) const {
            double aimAngle = 0;
            double closestDistance = 10000;
            bool enemyFound = false;
            const float towerCenterX = getPosition().x;
            const float towerCenterY = getPosition().y;

            for (const auto& enemy : enemies) {
                const double distance = std::sqrt(std::pow(towerCenterX - enemy->getPosition().position.x, 2)
                    + std::pow(towerCenterY - enemy->getPosition().position.y, 2));
                if (distance < closestDistance && doCirclesOverlap(*getRangeIndicator(), *enemy->getHitTexture()->getCircleDisplayEntity())) {
                    closestDistance = distance;
                    aimAngle = std::atan2(enemy->getPosition().position.y - towerCenterY, enemy->getPosition().position.x - towerCenterX);
                    enemyFound = true;
                }
            }
            return { aimAngle, enemyFound };
        }

        virtual std::vector<std::shared_ptr<Projectile>> generateProjectiles(double angle) = 0;
};



#endif //BASETOWER_H
