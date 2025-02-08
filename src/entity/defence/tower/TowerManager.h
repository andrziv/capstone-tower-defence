#ifndef TOWERMANAGER_H
#define TOWERMANAGER_H
#include <list>
#include <vector>
#include <memory>
#include "Tower.h"
#include "../projectile/ProjectileManager.h"
#include <SFML/Graphics.hpp>

class TowerManager {
    ProjectileManager projectileManager;
    std::list<std::shared_ptr<Tower>> towers;

public:
    void update() const {
        projectileManager.update();
    }

    void addTower(const std::shared_ptr<Tower>& newTower) {
        towers.push_back(newTower);
    }

    void addTowers(const std::vector<std::shared_ptr<Tower>>& newTowers) {
        for (const auto& tower : newTowers) {
            towers.push_back(tower);
        }
    }

    void removeTower(const std::shared_ptr<Tower>& tower) {
        towers.remove(tower);  
    }

    [[nodiscard]] std::vector<std::shared_ptr<Tower>> getTowers() const {
        return { towers.begin(), towers.end() };
    }

    void enemyInteractions(std::vector<std::shared_ptr<Enemy>> enemies) {
        for (const auto& tower : towers) {
            for (auto& projectile : tower->shootProjectile(enemies)) {
                projectileManager.addProjectile(projectile);
            }
        }
        projectileManager.enemyInteractions(enemies);
    }

    [[nodiscard]] std::vector<Projectile*> getActiveProjectiles() const {
        return projectileManager.getActiveProjectiles();
    }

    [[nodiscard]] std::vector<Projectile*> getInactiveProjectiles() const {
        return projectileManager.getInactiveProjectiles();
    }

    [[nodiscard]] std::vector<std::shared_ptr<Projectile>> getUndrawnProjectiles() {
        return projectileManager.getUndrawnProjectiles();
    }

    [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getDisplayEffects() const {
        return projectileManager.getDisplayEffects();
    }

    [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getCompletedDisplayEffects() const {
        return projectileManager.getCompletedDisplayEffects();
    }

    void removeInactiveProjectiles() {
        return projectileManager.removeInactiveProjectiles();
    }
};

#endif // TOWERMANAGER_H
