#ifndef BOMBTOWER_H
#define BOMBTOWER_H
#include "../BaseTower.h"
#include "../../../projectile/tower_projectile/bomb/BombProjectile.h"


class BombTower final : public BaseTower{
public:
    explicit BombTower(const sf::Vector2f& position)
        : BaseTower (position, 150, 540, 7.f, 200, "Bomb",sf::Color(40, 50, 40))  {
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

    std::string getPressurePattern() override {
        return R"~(^(00))~";
    }
};



#endif //BOMBTOWER_H
