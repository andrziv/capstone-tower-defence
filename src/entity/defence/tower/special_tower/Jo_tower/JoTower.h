#ifndef JOTOWER_H
#define JOTOWER_H

#include <memory>

#include "../BaseTower.h"
#include "../../Tower.h"
#include "../../../projectile/tower_projectile/basic/BasicProjectile.h"
#include "../../../../../texture/TowerSpriteInjector.h"

class Projectile;


class JoTower final : public BaseTower {
    std::shared_ptr<TowerSpriteInjector> towerSpriteInjector;

    public:
        explicit JoTower(const std::shared_ptr<TowerSpriteInjector> &towerSpriteInjector, const sf::Vector2f& position)
            : BaseTower (towerSpriteInjector->createArcherMaxIdleHitTexture(), position, 250, 270, 3.f, 150, "Dart", sf::Color::White)  {
            this->towerSpriteInjector = towerSpriteInjector;
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
            return R"~(^(000))~";
        }
};

#endif //JOTOWER_H
