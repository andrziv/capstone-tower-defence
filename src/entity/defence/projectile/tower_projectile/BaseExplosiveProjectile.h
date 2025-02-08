#ifndef BASEEXPLOSIVEPROJECTILE_H
#define BASEEXPLOSIVEPROJECTILE_H
#include "BaseNormalProjectile.h"


class BaseExplosiveProjectile : public BaseNormalProjectile {
    std::shared_ptr<CircleHitTexture> explosion;

public:

    BaseExplosiveProjectile(const float posX, const float posY, const float direction,
        const int damage, const int speed, const float explosionSize, const sf::Color explosionColor,
        const float size, const sf::Color color)
        : BaseNormalProjectile(posX, posY, direction, 1, damage, speed, size, color) {
        explosion = std::make_shared<CircleHitTexture>(CircleHitTexture());
        explosion->setDisplayEntityColor(explosionColor);
        explosion->setDisplayEntityRadius(explosionSize);
        explosion->setHitboxColor(sf::Color::Transparent);
        explosion->setHitboxRadius(explosionSize);
    }

    BaseExplosiveProjectile(const float posX, const float posY, const float direction)
        : BaseExplosiveProjectile(posX, posY, direction, 1, 10, 50.f,
            sf::Color(186, 86, 35), 8.f, sf::Color(70, 70, 70)) {
    }

    void handleEnemies(const std::vector<std::shared_ptr<Enemy>> &enemies) override {
        bool projectileExploded = false;
        for (const auto &enemy : enemies) {
            if (!isValid()) {
                break;
            }
            if (!enemy->isAlive()) {
                continue;
            }
            if (projectileExploded) {
                const auto enemyCollidingExplosion = doCirclesOverlap(*explosion->getCircleHitbox(), *enemy->getHitTexture()->getCircleHitbox());
                if (enemyCollidingExplosion) {
                    enemy->subtractHealth(getDamage());
                }
            } else if (!alreadyCollided(enemy)) {
                const auto enemyCollidingProjectile = doCirclesOverlap(*getHitTexture()->getCircleHitbox(), *enemy->getHitTexture()->getCircleHitbox());
                if (enemyCollidingProjectile) {
                    projectileExploded = true;
                    enemy->subtractHealth(getDamage());
                    collisions.push_back(enemy->getId());
                }
            }
        }
        if (projectileExploded) {
            setPierce(0);
        }
    }
};



#endif //BASEEXPLOSIVEPROJECTILE_H
