//
// Created by aki on 12/29/24.
//

#ifndef PROJECTILEMANAGER_H
#define PROJECTILEMANAGER_H
#include <list>
#include <vector>

#include "Projectile.h"
#include "../../enemy/Enemy.h"

class ProjectileManager {
    std::list<Projectile> projectiles;

    public:
        void update() const {
            for (auto projectile : projectiles) {
                if (projectile.isValid()) {
                    projectile.updatePosition();
                }
            }
        }

        void enemyInteractions(std::vector<Enemy> enemies) {
            for (auto projectile : projectiles) {
                for (auto enemy : enemies) {
                    if (projectile.isColliding(&enemy)) {
                        projectile.onCollision(&enemy);
                    }
                }
            }
        }

        void addProjectile(Projectile projectile) {
            projectiles.push_back(projectile);
        }

        std::vector<Projectile> getActiveProjectiles() const {
            std::vector<Projectile> active;
            for (auto projectile : projectiles) {
                if (projectile.isValid()) {
                    active.push_back(projectile);
                }
            }
            return active;
        }

        std::vector<Projectile> getInactiveProjectiles() const {
            std::vector<Projectile> inactive;
            for (auto projectile : projectiles) {
                if (!projectile.isValid()) {
                    inactive.push_back(projectile);
                }
            }
            return inactive;
        }

        void removeInactiveProjectiles() {
            for (auto projectile : projectiles) {
                if (projectile.isValid()) {
                    projectiles.remove(projectile);
                }
            }
        }
};



#endif //PROJECTILEMANAGER_H
