#ifndef JOTOWER_H
#define JOTOWER_H

#include <memory>

#include "../BaseTower.h"
#include "../../Tower.h"
#include "../../../projectile/tower_projectile/basic/BasicProjectile.h"

class Projectile;


class JoTower final : public BaseTower {
    public:
        explicit JoTower(const sf::Vector2f& position)
            : BaseTower (position, 250, 270, 3.f, sf::Color::Green)  {
        }

    protected:
        std::shared_ptr<Tower> copy() override {
            return std::make_shared<JoTower>(*this);
        }

        std::vector<std::shared_ptr<Projectile>> generateProjectiles(const double angle) override {
            std::vector<std::shared_ptr<Projectile>> projectiles;
            const float towerCenterX = getPosition().x;
            const float towerCenterY = getPosition().y;
            projectiles.push_back(std::make_shared<BasicProjectile>(BasicProjectile(towerCenterX, towerCenterY, static_cast<float>(angle), 3)));

            return projectiles;
        }

        std::string getPressurePattern() override {
            return R"~(^(0000))~";
        }
};

#endif //JOTOWER_H
