#ifndef PROJECTILEMANAGER_H
#define PROJECTILEMANAGER_H
#include <list>
#include <vector>

#include "Projectile.h"
#include "../../enemy/Enemy.h"
#include "tower_projectile/dev/DevProjectile.h"

class ProjectileManager {
    std::list<Projectile*> projectiles;

    public:
        // TODO: temp; just for testing atm
        ProjectileManager() {
            auto *projectile1 = new DevProjectile(2, 1, 0, 782, 412, sf::Color(255, 98, 0), 4.f);
            auto *projectile2 = new DevProjectile(1, 1, 0, 985, 586);
            auto *projectile3 = new DevProjectile(4, 1, 0, 527, 615, sf::Color(11, 91, 92), 6.f);
            auto *projectile4 = new DevProjectile(4, 1, 0, 293, 787, sf::Color(11, 91, 92), 6.f);
            addProjectile(projectile1);
            addProjectile(projectile2);
            addProjectile(projectile3);
            addProjectile(projectile4);
        }

        void update() const {
            for (const auto &projectile : projectiles) {
                if (projectile->isValid()) {
                    projectile->updatePosition();
                }
            }
        }

        void enemyInteractions(const std::vector<Enemy*>& enemies) const {
            for (const auto &projectile : projectiles) {
                if (projectile->isValid()) {
                    for (const auto &enemy : enemies) {
                        if (enemy->isAlive() && projectile->isColliding(enemy)) {
                            projectile->onCollision(enemy);
                        }
                    }
                }
            }
        }

        void addProjectile(Projectile* projectile) {
            projectiles.push_back(projectile);
        }

        [[nodiscard]] std::vector<Projectile*> getActiveProjectiles() const {
            std::vector<Projectile*> active;
            for (const auto& projectile : projectiles) {
                if (projectile->isValid()) {
                    active.push_back(projectile);
                }
            }
            return active;
        }

        [[nodiscard]] std::vector<Projectile*> getInactiveProjectiles() const {
            std::vector<Projectile*> inactive;
            for (const auto& projectile : projectiles) {
                if (!projectile->isValid()) {
                    inactive.push_back(projectile);
                }
            }
            return inactive;
        }

        void removeInactiveProjectiles() {
            for (const auto& projectile : projectiles) {
                if (projectile->isValid()) {
                    projectiles.remove(projectile);
                }
            }
        }
};



#endif //PROJECTILEMANAGER_H
