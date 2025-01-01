#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "entity/defence/projectile/Projectile.h"
#include "entity/defence/projectile/ProjectileManager.h"
#include "entity/defence/tower/TowerManager.h"
#include "entity/enemy/Enemy.h"
#include "entity/enemy/EnemyManager.h"


class Tower;

class GameManager {
    EnemyManager enemyManager;
    TowerManager towerManager;

public:
    GameManager() = default;

    void update() {
        enemyManager.update();
        towerManager.update();
        towerManager.enemyInteractions(enemyManager.getAliveEnemies());
        enemyManager.replaceDeadEnemiesWithChildren();
    }

    [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getDrawables() const {
        std::vector<std::shared_ptr<sf::Drawable>> drawables;
        drawables.push_back(enemyManager.getEnemyPath());

        const std::vector<Enemy*> enemies = enemyManager.getAliveEnemies();
        const std::vector<Projectile*> projectiles = towerManager.getActiveProjectiles();
        for (const auto enemy : enemies) {
            drawables.push_back(enemy->getHitTexture()->getDisplayEntity());
        }
        for (const auto projectile : projectiles) {
            drawables.push_back(projectile->getHitTexture()->getDisplayEntity());
        }
        return drawables;
    }

    // TODO: reminder to replace this with a more robust system that preferably doesn't require a completely separate set of lists
    [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getNewDrawables() {
        const std::vector<Enemy*> enemies = enemyManager.getUndrawnEnemies();
        std::vector<std::shared_ptr<sf::Drawable>> drawables;
        for (const auto enemy : enemies) {
            drawables.push_back(enemy->getHitTexture()->getDisplayEntity());
        }
        return drawables;
    }

    [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getRemovableDrawables() const {
        const std::vector<Enemy*> enemies = enemyManager.getDeadEnemies();
        const std::vector<Projectile*> projectiles = towerManager.getInactiveProjectiles();
        std::vector<std::shared_ptr<sf::Drawable>> drawables;
        for (const auto enemy : enemies) {
            drawables.push_back(enemy->getHitTexture()->getDisplayEntity());
        }
        for (const auto projectile : projectiles) {
            drawables.push_back(projectile->getHitTexture()->getDisplayEntity());
        }
        return drawables;
    }

    void cleanup() {
        enemyManager.removeDeadEnemies();
        towerManager.removeInactiveProjectiles();
    }

    // TODO: temp; just for testing atm
    void shrinkEnemyPath() const {
        enemyManager.shrinkEnemyPath();
    }
};



#endif //GAMEMANAGER_H
