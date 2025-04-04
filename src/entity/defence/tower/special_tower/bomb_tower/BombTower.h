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
        : BaseTower(towerSpriteInjector->createCannonMaxIdleHitTexture(), position, 1.8f, 150, 540, 7.f, 200, "Bomb",
                    sf::Color::White) {
        this->towerSpriteInjector = towerSpriteInjector;
        this->projectileSpriteInjector = projectileSpriteInjector;
    }

protected:
    std::shared_ptr<Tower> copy() override {
        return std::make_shared<BombTower>(*this);
    }

    std::vector<std::shared_ptr<Projectile>> generateProjectiles(const double angle) override {
        std::vector<std::shared_ptr<Projectile>> projectiles;

        if (getUpgradeValue() == 1) {
            const float towerCenterX = getPosition().x;
            const float towerCenterY = getPosition().y;
            projectiles.push_back(
                std::make_shared<BombProjectile>(BombProjectile(projectileSpriteInjector->createExplosionHitTexture(),
                                                                towerCenterX,
                                                                towerCenterY, static_cast<float>(angle), 0)));
        } else if (getUpgradeValue() == 2) {
            const float towerCenterX = getPosition().x;
            const float towerCenterY = getPosition().y;
            projectiles.push_back(
                std::make_shared<BombProjectile>(BombProjectile(projectileSpriteInjector->createExplosionHitTexture(),
                                                                towerCenterX,
                                                                towerCenterY, static_cast<float>(angle), 200)));
        } else if (getUpgradeValue() == 3) {
            const float towerCenterX = getPosition().x;
            const float towerCenterY = getPosition().y;
            projectiles.push_back(
                std::make_shared<BombProjectile>(BombProjectile(projectileSpriteInjector->createExplosionHitTexture(),
                                                                towerCenterX,
                                                                towerCenterY, static_cast<float>(angle), 2000)));
        }

        return projectiles;
    }

    std::string getPressurePattern() override {
        return R"~(^(00))~";
    }

    void upgradeTower(const int upgradeValue) override {
        if (upgradeValue == 3) {
            setAttackSpeed(4);
            setDamage(400);
        } else if (upgradeValue == 2) {
            setAttackSpeed(5);
            setDamage(300);
        }
    }
};


#endif //BOMBTOWER_H
