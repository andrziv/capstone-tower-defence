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

        void enemyInteractions(const std::vector<std::shared_ptr<Enemy>> &enemies) const {
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
