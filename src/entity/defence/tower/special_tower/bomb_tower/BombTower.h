#ifndef BOMBTOWER_H
#define BOMBTOWER_H
#include "../BaseTower.h"
#include "../../../../../texture/ProjectileSpriteInjector.h"
#include "../../../../../texture/TowerSpriteInjector.h"
#include "../../../projectile/tower_projectile/bomb/BombProjectile.h"


class BombTower final : public BaseTower {
    std::shared_ptr<TowerSpriteInjector> towerSpriteInjector;
    std::shared_ptr<ProjectileSpriteInjector> projectileSpriteInjector;

public:
    BombTower(const std::shared_ptr<TowerSpriteInjector> &towerSpriteInjector,
              const std::shared_ptr<ProjectileSpriteInjector> &projectileSpriteInjector, const sf::Vector2f &position)
        : BaseTower(towerSpriteInjector->createCannonMaxIdleHitTexture(), position, 1.8f, 150, 540, 7.f, 200, "Bomb", sf::Color::White) {
        this->towerSpriteInjector = towerSpriteInjector;
        this->projectileSpriteInjector = projectileSpriteInjector;
    }

protected:
    std::shared_ptr<Tower> copy() override {
        return std::make_shared<BombTower>(*this);
    }

    std::vector<std::shared_ptr<Projectile> > generateProjectiles(const double angle) override {
        std::vector<std::shared_ptr<Projectile> > projectiles;
        const float towerCenterX = getPosition().x;
        const float towerCenterY = getPosition().y;
        projectiles.push_back(
            std::make_shared<BombProjectile>(BombProjectile(projectileSpriteInjector->createExplosionHitTexture(), towerCenterX,
                                                            towerCenterY, static_cast<float>(angle))));

        return projectiles;
    }

    std::string getPressurePattern() override {
        return R"~(^(00))~";
    }
};


#endif //BOMBTOWER_H
