#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "entity/defence/projectile/Projectile.h"
#include "entity/defence/projectile/ProjectileManager.h"
#include "entity/defence/tower/TowerManager.h"
#include "entity/enemy/Enemy.h"
#include "entity/enemy/EnemyManager.h"
#include "entity/enemy/waves/WaveLoader.h"


class Tower;

class GameManager {
    EnemyManager enemyManager;
    TowerManager towerManager;
    WaveLoader waveLoader = WaveLoader("../../src/resources/waves/dev-waves.json");
    std::chrono::steady_clock::time_point time_start = std::chrono::steady_clock::now();

    std::vector<std::shared_ptr<Enemy>> nextEnemyWave() {
        std::vector<std::shared_ptr<Enemy>> enemies;
        for (const auto& [timeToSpawn, enemy] : waveLoader.getNextWave()) {
            enemies.push_back(enemy);
        }
        return enemies;
    }

    void summonNewEnemies() {
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(end - time_start).count() >= 10) {
            enemyManager.addEnemies(nextEnemyWave());
            time_start = std::chrono::steady_clock::now();
        }
    }

public:
    GameManager() = default;

    void update() {
        enemyManager.update();
        towerManager.update();
        towerManager.enemyInteractions(enemyManager.getAliveEnemies());
        enemyManager.replaceDeadEnemiesWithChildren();
        summonNewEnemies();
    }

    [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getDrawables() const {
        std::vector<std::shared_ptr<sf::Drawable>> drawables;
        drawables.push_back(enemyManager.getEnemyPath());

        const std::vector<std::shared_ptr<Enemy>> enemies = enemyManager.getAliveEnemies();
        const std::vector<Projectile*> projectiles = towerManager.getActiveProjectiles();
        for (const auto& enemy : enemies) {
            drawables.push_back(enemy->getHitTexture()->getDisplayEntity());
        }
        for (const auto projectile : projectiles) {
            drawables.push_back(projectile->getHitTexture()->getDisplayEntity());
        }
        return drawables;
    }

    // TODO: reminder to replace this with a more robust system that preferably doesn't require a completely separate set of lists
    [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getNewDrawables() {
        const std::vector<std::shared_ptr<Enemy>> enemies = enemyManager.getUndrawnEnemies();
        std::vector<std::shared_ptr<sf::Drawable>> drawables;
        for (const auto& enemy : enemies) {
            drawables.push_back(enemy->getHitTexture()->getDisplayEntity());
        }
        return drawables;
    }

    [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getRemovableDrawables() const {
        const std::vector<std::shared_ptr<Enemy>> enemies = enemyManager.getDeadEnemies();
        const std::vector<Projectile*> projectiles = towerManager.getInactiveProjectiles();
        std::vector<std::shared_ptr<sf::Drawable>> drawables;
        for (const auto& enemy : enemies) {
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

    [[nodiscard]] int getMaxWaveNumber() const {
        return waveLoader.getMaxWaves();
    }

    [[nodiscard]] int getCurrentWaveNumber() const {
        return waveLoader.getCurrentWave();
    }
};



#endif //GAMEMANAGER_H
