#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <queue>
#include <memory>

#include "entity/defence/projectile/Projectile.h"
#include "entity/defence/tower/TowerManager.h"
#include "entity/enemy/Enemy.h"
#include "entity/enemy/EnemyManager.h"
#include "entity/enemy/waves/WaveLoader.h"
#include "selection/TowerSelector.h"

class Tower;

class GameManager {
    int playerHealth = 1000;
    int playerBalance = 4000;
    EnemyManager enemyManager;
    TowerManager towerManager;
    TowerSelector towerSelector;
    WaveLoader waveLoader = WaveLoader("../../src/resources/waves/dev-waves.json");
    std::chrono::steady_clock::time_point waveTimeStart = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point interWaveTimeStart = std::chrono::steady_clock::now();
    std::queue<std::pair<double, std::pair<double, std::vector<std::shared_ptr<Enemy>>>>> enemySpawnTimeQueue;
    std::pair<double, std::pair<double, std::vector<std::shared_ptr<Enemy>>>> nextEnemySummonSet;
    bool waveLoadingPaused = true;

    void checkAndLoadNewEnemies() {
        const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        const auto timeDiff = std::chrono::duration_cast<std::chrono::seconds>(end - waveTimeStart).count();
        if (enemySpawnTimeQueue.empty() && enemyManager.getNumberOfAliveEnemies() == 0 && waveLoadingPaused) {
            //
            if (waveLoader.getCurrentWave() < waveLoader.getMaxWaves()) {
                int rewardMoney = 1000;
                playerBalance += rewardMoney;
                //
                waveTimeStart = std::chrono::steady_clock::now();
                waveLoadingPaused = false;
            }
        }
        if (timeDiff >= 5 && !waveLoadingPaused) {
            loadNextEnemyWave();
            waveLoadingPaused = true;
        }
    }

    void loadNextEnemyWave() {
        std::map<double, std::pair<double, std::vector<std::shared_ptr<Enemy>>>> tempSpawnMap;
        double previousSpawnTime = 0;
        for (const auto& [total_time, enemy_info] : waveLoader.getNextWave()) {
            const auto spawnPercent = enemy_info.first;
            auto spawnTime = total_time * (spawnPercent / 100.0);
            auto spawnInterval = spawnTime - previousSpawnTime;
            previousSpawnTime = spawnTime;
            auto enemy = enemy_info.second;
            auto spawnPair = tempSpawnMap.find(spawnTime);
            if (spawnPair != tempSpawnMap.end()) {
                spawnPair->second.second.push_back(enemy);
            } else {
                std::vector<std::shared_ptr<Enemy>> enemies;
                enemies.push_back(enemy);
                tempSpawnMap[spawnTime] = std::pair(spawnInterval, enemies);
            }
        }
        for (const auto& [spawnTime, enemyList] : tempSpawnMap) {
            enemySpawnTimeQueue.emplace(spawnTime, enemyList);
        }
    }

    void summonNewEnemies() {
        const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        if (enemySpawnTimeQueue.empty()) {
            return;
        }
        auto [spawnTime, enemySpawnInfo] = enemySpawnTimeQueue.front();
        const auto timeDifference = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(end - interWaveTimeStart).count()) / 1000;
        if (timeDifference >= enemySpawnInfo.first && !enemySpawnInfo.second.empty()) {
            enemyManager.addEnemies(enemySpawnInfo.second);
            enemySpawnTimeQueue.pop();
            interWaveTimeStart = std::chrono::steady_clock::now();
        }
    }

    void penalizeForFinishedEnemies() {
        for ([[maybe_unused]] const auto& enemy : enemyManager.getEnemiesAtEndOfPath()) {
            playerHealth--;
        }
    }

public:
    GameManager() = default;

    void update() {
        enemyManager.update();
        towerManager.update();
        towerManager.enemyInteractions(enemyManager.getAliveEnemies());
        enemyManager.replaceDeadEnemiesWithChildren();
        checkAndLoadNewEnemies();
        summonNewEnemies();
        penalizeForFinishedEnemies();
    }

    bool attemptSelectingTower(const sf::Vector2i& mousePosition){
        return towerSelector.attemptSelectingTower(mousePosition);
    }

    std::shared_ptr<Tower> attemptSelectingPlacedTower(const sf::Vector2i &mousePosition){
        return TowerSelector::attemptSelectingPlacedTower(mousePosition, towerManager.getTowers());
    }

    void dragSelectedTower(const sf::Vector2i& mousePosition) {
        towerSelector.dragSelectedTower(mousePosition);
        const auto towerOverlap = towerSelector.doesSelectedTowerOverlap(towerManager.getTowers());
        const auto towerPosX = towerSelector.getSelectedTower()->getPosition().x;
        const auto menuStartX = SEL_MENU_START_X - towerSelector.getSelectedTower()->getHitTexture()->getRectHitbox()->getSize().x / 2;
        const auto outOfBounds = towerPosX > menuStartX;
        if (towerOverlap || outOfBounds) {
            towerSelector.getSelectedTower()->invalidateRangeIndicator();
        } else {
            towerSelector.getSelectedTower()->validateRangeIndicator();
        }
    }

    void deselectTower() {
        towerSelector.deselectTower();
    }

    [[nodiscard]] bool isTowerAlreadySelected() const {
        return towerSelector.isStillSelected();
    }

    bool addTower(const std::shared_ptr<Tower>& tower) {
        if (playerBalance >= tower->getCost()) {
            const auto overlap = towerSelector.doesSelectedTowerOverlap(towerManager.getTowers());
            const auto towerPosX = towerSelector.getSelectedTower()->getPosition().x;
            const auto menuStartX = SEL_MENU_START_X - towerSelector.getSelectedTower()->getHitTexture()->getRectHitbox()->getSize().x / 2;
            const auto outOfBounds = towerPosX > menuStartX;
            if (!(overlap || outOfBounds)) {
                towerManager.addTower(tower);
                playerBalance -= tower->getCost();
                return true;
            }
        }
        return false;
    }

    void removeTower(const std::shared_ptr<Tower>& tower) {
        towerManager.removeTower(tower);
    }

    void sellTower(const std::shared_ptr<Tower>& tower) {
        int sellPrice = tower->getCost() / 2; // Refund half of the tower's cost
        playerBalance += sellPrice;
        removeTower(tower);
    }

    std::shared_ptr<Tower> getHoveredTower() {
        return towerSelector.getSelectedTower();
    }

    std::shared_ptr<sf::Drawable> getHoveredTowerDrawable() {
        return towerSelector.getSelectedTower()->getHitTexture()->getDisplayEntity();
    }

    [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getAvailTowerDrawables() const {
        std::vector<std::shared_ptr<sf::Drawable>> drawables;
        const std::vector<std::shared_ptr<Tower>> towers = towerSelector.getAvailableTowers();
        for (const auto& tower : towers) {
            drawables.push_back(tower->getHitTexture()->getDisplayEntity());
        }

        return drawables;
    }

    [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getDrawables() const {
        std::vector<std::shared_ptr<sf::Drawable>> drawables;
        drawables.push_back(enemyManager.getVisualEnemyPath());

        const std::vector<std::shared_ptr<Enemy>> enemies = enemyManager.getAliveEnemies();
        const std::vector<Projectile*> projectiles = towerManager.getActiveProjectiles();
        const std::vector<std::shared_ptr<Tower>> towers = towerManager.getTowers();

        for (const auto& enemy : enemies) {
            drawables.push_back(enemy->getHitTexture()->getDisplayEntity());
        }
        for (const auto projectile : projectiles) {
            drawables.push_back(projectile->getHitTexture()->getDisplayEntity());
        }
        for (const auto& tower : towers) {
            drawables.push_back(tower->getHitTexture()->getDisplayEntity());
        }
        
        return drawables;
    }

    [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getNewDrawables() {
        const std::vector<std::shared_ptr<Enemy>> enemies = enemyManager.getUndrawnEnemies();
        const std::vector<std::shared_ptr<Projectile>> projectiles = towerManager.getUndrawnProjectiles();
        std::vector<std::shared_ptr<sf::Drawable>> drawables;
        for (const auto& enemy : enemies) {
            drawables.push_back(enemy->getHitTexture()->getDisplayEntity());
        }
        for (const auto& projectile : projectiles) {
            drawables.push_back(projectile->getHitTexture()->getDisplayEntity());
        }
        for (const auto& projectileDisplayEffect : towerManager.getDisplayEffects()) {
            drawables.push_back(projectileDisplayEffect);
        }
        return drawables;
    }

    [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getRemovableDrawables() const {
        const std::vector<std::shared_ptr<Enemy>> enemies = enemyManager.getDeadEnemies();
        const std::vector<std::shared_ptr<Enemy>> enemies2 = enemyManager.getEnemiesAtEndOfPath();
        const std::vector<Projectile*> projectiles = towerManager.getInactiveProjectiles();
        std::vector<std::shared_ptr<sf::Drawable>> drawables;
        for (const auto& enemy : enemies) {
            drawables.push_back(enemy->getHitTexture()->getDisplayEntity());
        }
        for (const auto& enemy : enemies2) {
            drawables.push_back(enemy->getHitTexture()->getDisplayEntity());
        }
        for (const auto projectile : projectiles) {
            drawables.push_back(projectile->getHitTexture()->getDisplayEntity());
        }
        for (const auto& projectileDisplayEffect : towerManager.getCompletedDisplayEffects()) {
            drawables.push_back(projectileDisplayEffect);
        }
        return drawables;
    }

    void cleanup() {
        enemyManager.removeDeadEnemies();
        towerManager.removeInactiveProjectiles();
    }

    void shrinkEnemyPath() const {
        enemyManager.shrinkEnemyPath();
    }

    [[nodiscard]] int getMaxWaveNumber() const {
        return waveLoader.getMaxWaves();
    }

    [[nodiscard]] int getCurrentWaveNumber() const {
        return waveLoader.getCurrentWave();
    }

    [[nodiscard]] int getPlayerHealth() const {
        return playerHealth;
    }

    [[nodiscard]] int getPlayerBalance() const {
        return playerBalance;
    }
};

#endif //GAMEMANAGER_H

