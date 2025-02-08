//
// Created by aki on 2/7/25.
//

#ifndef BOMBTOWER_H
#define BOMBTOWER_H
#include "../BaseTower.h"
#include "../../../projectile/tower_projectile/bomb/BombProjectile.h"


class BombTower final : public BaseTower{
public:
    explicit BombTower(const sf::Vector2f& position)
        : BaseTower (position, 150, 540, 7.f, sf::Color(40, 50, 40))  {
    }

protected:
    std::shared_ptr<Tower> copy() override {
        return std::make_shared<BombTower>(*this);
    }

    std::vector<std::shared_ptr<Projectile>> generateProjectiles(const double angle) override {
        std::vector<std::shared_ptr<Projectile>> projectiles;
        const float towerCenterX = getPosition().x;
        const float towerCenterY = getPosition().y;
        projectiles.push_back(std::make_shared<BombProjectile>(BombProjectile(towerCenterX, towerCenterY, static_cast<float>(angle))));

        return projectiles;
    }
};



#endif //BOMBTOWER_H
