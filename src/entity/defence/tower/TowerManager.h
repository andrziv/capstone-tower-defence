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
        void update() const {
            projectileManager.update();
        }

        std::vector<Projectile> getActiveProjectiles() const {
            return projectileManager.getActiveProjectiles();
        }

        std::vector<Projectile> getInactiveProjectiles() const {
            return projectileManager.getInactiveProjectiles();
        }

        void removeInactiveProjectiles() {
            return projectileManager.removeInactiveProjectiles();
        }
};



#endif //TOWERMANAGER_H
