#ifndef BASEEXPLOSIVEPROJECTILE_H
#define BASEEXPLOSIVEPROJECTILE_H
#include <chrono>
#include <list>

#include "BaseNormalProjectile.h"


class BaseExplosiveProjectile : public BaseNormalProjectile {
    std::shared_ptr<CircleHitTexture> explosion;
    std::list<std::pair<std::shared_ptr<sf::Drawable>, std::chrono::steady_clock::time_point>> newDisplayEffects;
    std::list<std::pair<std::shared_ptr<sf::Drawable>, std::chrono::steady_clock::time_point>> oldDisplayEffects;
    const double effectExpireTimeSecs = 0.5;

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

    BaseExplosiveProjectile(const float posX, const float posY, const float direction,
        const int speed, const float explosionSize, const float size)
        : BaseExplosiveProjectile(posX, posY, direction, 1, speed, explosionSize,
            sf::Color(255, 72, 0, 100), size, sf::Color(70, 70, 70)) {
    }

    BaseExplosiveProjectile(const float posX, const float posY, const float direction)
        : BaseExplosiveProjectile(posX, posY, direction, 1, 10, 50.f,
            sf::Color(255, 72, 0, 100), 8.f, sf::Color(70, 70, 70)) {
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
            if (!alreadyCollided(enemy)) {
                const auto enemyCollidingProjectile = doCirclesOverlap(*getHitTexture()->getCircleHitbox(), *enemy->getHitTexture()->getCircleHitbox());
                if (enemyCollidingProjectile) {
                    projectileExploded = true;
                    enemy->subtractHealth(getDamage());
                    collisions.push_back(enemy->getId());
                    explosion->setPosition(getPos());
                    newDisplayEffects.emplace_back(std::pair(explosion->getCircleDisplayEntity(), std::chrono::steady_clock::now()));
                    break;
                }
            }
        }
        if (projectileExploded) {
            for (const auto &enemy : enemies) {
                if (!enemy->isAlive()) {
                    continue;
                }
                const auto enemyCollidingExplosion = doCirclesOverlap(*explosion->getCircleHitbox(), *enemy->getHitTexture()->getCircleHitbox());
                if (enemyCollidingExplosion) {
                    enemy->subtractHealth(getDamage());
                }
            }
            setPierce(0);
        }
    }

    bool hasDisplayEffects() override {
        return !oldDisplayEffects.empty() || !newDisplayEffects.empty();
    }

    std::vector<std::shared_ptr<sf::Drawable>> getDisplayEffects() override {
        std::vector<std::shared_ptr<sf::Drawable>> drawables;
        for (const auto& pair : newDisplayEffects) {
            const auto& [drawable, time] = pair;
            drawables.push_back(drawable);
            oldDisplayEffects.push_back(pair);
        }
        newDisplayEffects.clear();
        return drawables;
    }

    std::vector<std::shared_ptr<sf::Drawable>> getCompletedDisplayEffects() override {
        const auto currentTime = std::chrono::steady_clock::now();
        std::vector<std::shared_ptr<sf::Drawable>> drawables;
        std::vector<std::pair<std::shared_ptr<sf::Drawable>, std::chrono::steady_clock::time_point>> toRemove;
        for (const auto& pair : oldDisplayEffects) {
            const auto& [drawable, time] = pair;
            const auto timeDifference = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - time).count()) / 1000;
            if (timeDifference >= effectExpireTimeSecs) {
                drawables.push_back(drawable);
                toRemove.push_back(pair);
            }
        }
        for (const auto& pair : toRemove) {
            oldDisplayEffects.remove(pair);
        }
        newDisplayEffects.remove_if([currentTime, this](const std::pair<std::shared_ptr<sf::Drawable>, std::chrono::steady_clock::time_point>& pair) {
            const auto& [drawable, time] = pair;
            const auto timeDifference = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - time).count()) / 1000;
            return timeDifference >= effectExpireTimeSecs;
        });
        return drawables;
    }
};



#endif //BASEEXPLOSIVEPROJECTILE_H
