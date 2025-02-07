#ifndef JOTOWER_H
#define JOTOWER_H


#include <string>
#include <memory>

#include "../../Tower.h"
#include "../../../projectile/tower_projectile/dev/DevProjectile.h"
#include "SFML/Graphics/Vertex.hpp"
#include "SFML/Graphics.hpp"

class Projectile;


class JoTower final : public Tower {
    std::chrono::steady_clock::time_point shootStart = std::chrono::steady_clock::now();
    public:
        explicit JoTower(const sf::Vector2f& position) : Tower (position, 100, 0, 3.f)  {
            getHitTexture()->setDisplayEntityColor(sf::Color::Green);
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
            if (timeDiff >= getAttackSpeed()) {
                double aimAngle = 0;
                double closestDistance = 10000;
                bool enemyFound = false;

                for (const auto& enemy : enemies) {
                    const double distance = std::sqrt(std::pow(getPosition().x - enemy->getPosition().position.x, 2)
                        + std::pow(getPosition().y - enemy->getPosition().position.y, 2));
                    if (distance < closestDistance) {
                        closestDistance = distance;
                        aimAngle = atan2((enemy->getPosition().position.y - getPosition().y), (enemy->getPosition().position.x - getPosition().x));
                        enemyFound = true;
                    }
                }
                if (enemyFound){
                    projectiles.push_back(std::make_shared<DevProjectile>(DevProjectile(2, 1, 20, getPosition().x, getPosition().y, sf::Color(255, 98, 0), 4.f, aimAngle)));
                }
                shootStart = std::chrono::steady_clock::now();
            }
            return projectiles;
        }

    protected:
        std::shared_ptr<Tower> copy() override {
            return std::make_shared<JoTower>(*this);
        }
};

#endif //JOTOWER_H
