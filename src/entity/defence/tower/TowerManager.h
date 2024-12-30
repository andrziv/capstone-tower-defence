//
// Created by aki on 12/29/24.
//

#ifndef TOWERMANAGER_H
#define TOWERMANAGER_H
#include <list>

#include "Tower.h"
#include "../projectile/ProjectileManager.h"


class TowerManager {
    ProjectileManager projectileManager;
    std::list<Tower> towers;

    public:
        void update() {
            projectileManager.update();
        }

        void enemyInteractions(const std::vector<Enemy*> &enemies) const {
            projectileManager.enemyInteractions(enemies);
        }

        [[nodiscard]] std::vector<Projectile*> getActiveProjectiles() const {
            return projectileManager.getActiveProjectiles();
        }

        [[nodiscard]] std::vector<Projectile*> getInactiveProjectiles() const {
            return projectileManager.getInactiveProjectiles();
        }

        void removeInactiveProjectiles() {
            return projectileManager.removeInactiveProjectiles();
        }
};



#endif //TOWERMANAGER_H
