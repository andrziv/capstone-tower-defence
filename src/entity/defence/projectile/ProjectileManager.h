#ifndef PROJECTILEMANAGER_H
#define PROJECTILEMANAGER_H
#include <list>
#include <memory>
#include <vector>

#include "Projectile.h"
#include "../../enemy/Enemy.h"
#include "tower_projectile/dev/DevProjectile.h"

class ProjectileManager {
    std::list<std::unique_ptr<Projectile>> projectiles;

    public:
        // TODO: temp; just for testing atm
        ProjectileManager() {
            std::unique_ptr<Projectile> projectile1 = std::make_unique<DevProjectile>(DevProjectile(2, 1, 0, 782, 412, sf::Color(255, 98, 0), 4.f));
            std::unique_ptr<Projectile> projectile2 = std::make_unique<DevProjectile>(DevProjectile(1, 1, 0, 985, 586));
            std::unique_ptr<Projectile> projectile3 = std::make_unique<DevProjectile>(DevProjectile(4, 1, 0, 527, 615, sf::Color(11, 91, 92), 6.f));
            std::unique_ptr<Projectile> projectile4 = std::make_unique<DevProjectile>(DevProjectile(4, 1, 0, 293, 787, sf::Color(11, 91, 92), 6.f));
            std::unique_ptr<Projectile> projectile5 = std::make_unique<DevProjectile>(DevProjectile(100, 10, 0, 293, 615, sf::Color(133, 0, 117), 10.f));

            addProjectile(projectile1);
            addProjectile(projectile2);
            addProjectile(projectile3);
            addProjectile(projectile4);
            addProjectile(projectile5);
        }

        void update() const {
            for (const auto &projectile : projectiles) {
                if (projectile->isValid()) {
                    projectile->updatePosition();
                }
            }
        }

        void enemyInteractions(const std::vector<std::shared_ptr<Enemy>>& enemies) const {
            for (const auto &projectile : projectiles) {
                for (const auto &enemy : enemies) {
                    if (!projectile->isValid()) {
                        break;
                    }
                    if (!enemy->isAlive()) {
                        continue;
                    }
                    if (projectile->isColliding(enemy)) {
                        projectile->onCollision(enemy);
                    }
                }
            }
        }

        void addProjectile(std::unique_ptr<Projectile>& projectile) {
            projectiles.push_back(std::move(projectile));
        }

        [[nodiscard]] std::vector<Projectile*> getActiveProjectiles() const {
            std::vector<Projectile*> active;
            for (const auto& projectile : projectiles) {
                if (projectile->isValid()) {
                    active.push_back(projectile.get());
                }
            }
            return active;
        }

        [[nodiscard]] std::vector<Projectile*> getInactiveProjectiles() const {
            std::vector<Projectile*> inactive;
            for (auto& projectile : projectiles) {
                if (!projectile->isValid()) {
                    inactive.push_back(projectile.get());
                }
            }
            return inactive;
        }

        void removeInactiveProjectiles() {
            projectiles.remove_if([](const std::unique_ptr<Projectile> &projectile) {
                return !projectile->isValid();
            });
        }
};



#endif //PROJECTILEMANAGER_H
