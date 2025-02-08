#ifndef RADIALSHOOTERTOWER_H
#define RADIALSHOOTERTOWER_H

#include "../BaseTower.h"
#include "../../Tower.h"
#include "../../../projectile/tower_projectile/basic/BasicProjectile.h"


class RadialShooterTower final : public BaseTower {

    const double bulletSpreadAngleDeg = 60;

    public:
        explicit RadialShooterTower(const sf::Vector2f& position)
            : BaseTower (position, 100, 370, 4.f, sf::Color(252, 152, 3))  {
        }

    protected:
        std::shared_ptr<Tower> copy() override {
            return std::make_shared<RadialShooterTower>(*this);
        }

        std::pair<double, bool> calculateAimAngle(std::vector<std::shared_ptr<Enemy>>& enemies) const override {
            bool enemyFound = false;

            for (const auto& enemy : enemies) {
                if (doCirclesOverlap(*getRangeIndicator(), *enemy->getHitTexture()->getCircleDisplayEntity())) {
                    enemyFound = true;
                    break;
                }
            }
            return { 0, enemyFound };
        }

        std::vector<std::shared_ptr<Projectile>> generateProjectiles(const double angle) override {
            const int shots = static_cast<int>(360 / bulletSpreadAngleDeg);
            std::vector<std::shared_ptr<Projectile>> projectiles;
            const float towerCenterX = getPosition().x;
            const float towerCenterY = getPosition().y;

            for (int i = 0; i < shots; i++) {
                const double bulletAngleRads = angle + i * (bulletSpreadAngleDeg * M_PI / 180);
                projectiles.push_back(std::make_shared<BasicProjectile>(BasicProjectile(towerCenterX, towerCenterY, static_cast<float>(bulletAngleRads))));
            }

            return projectiles;
        }
};



#endif //RADIALSHOOTERTOWER_H
