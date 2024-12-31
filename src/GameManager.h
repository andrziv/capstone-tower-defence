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

    void update() const {
        enemyManager.update();
        towerManager.update();
        towerManager.enemyInteractions(enemyManager.getAliveEnemies());
    }

    [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getDrawables() const {
        const std::vector<Enemy*> enemies = enemyManager.getAliveEnemies();
        const std::vector<Projectile*> projectiles = towerManager.getActiveProjectiles();
        std::vector<std::shared_ptr<sf::Drawable>> drawables;
        for (const auto enemy : enemies) {
            drawables.push_back(enemy->getHitTexture()->getDisplayEntity());
        }
        for (const auto projectile : projectiles) {
            drawables.push_back(projectile->getHitTexture()->getDisplayEntity());
        }
        drawables.push_back(enemyManager.getEnemyPath());
        return drawables;
    }

    // TODO: temp; just for testing atm
    void shrinkEnemyPath() const {
        enemyManager.shrinkEnemyPath();
    }
};



#endif //GAMEMANAGER_H
