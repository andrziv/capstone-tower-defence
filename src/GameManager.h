//
// Created by aki on 12/29/24.
//

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
    GameManager() {}

    void update() {
        enemyManager.update();
        towerManager.update();
    }

    std::vector<sf::Drawable*> getDrawables() const {
        const std::vector<Enemy> enemies = enemyManager.getAliveEnemies();
        const std::vector<Projectile> projectiles = towerManager.getActiveProjectiles();
        std::vector<sf::Drawable*> drawables;
        for (auto enemy : enemies) {
            drawables.push_back(enemy.getHitTexture()->getDisplayEntity());
        }
        for (auto projectile : projectiles) {
            drawables.push_back(projectile.getHitTexture().getDisplayEntity());
        }
        drawables.push_back(enemyManager.getEnemyPath());
        return drawables;
    }

    // temp; just for testing atm
    void shrinkEnemyPath() const {
        enemyManager.shrinkEnemyPath();
    }
};



#endif //GAMEMANAGER_H
