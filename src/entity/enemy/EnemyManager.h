#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H
#include <list>

#include "Enemy.h"
#include "special_enemy/dev/DevEnemy.h"
#include "special_enemy/dev/LargeDevEnemy.h"


class EnemyManager {
    std::list<Enemy*> enemies;
    std::list<Enemy*> undrawnEnemies;
    std::shared_ptr<sf::VertexArray> enemyPath;

    void addChildEnemy(Enemy* parentEnemy, const std::vector<Enemy*>& childEnemy) {
        for (Enemy* child : childEnemy) {
            child->setPosition(parentEnemy->getPosition().position);
            child->setTargetNode(parentEnemy->getTargetNode());
            child->setId(parentEnemy->getId());
            addEnemy(child);
        }
    }

    public:
        // TODO: temp; just for testing atm
        EnemyManager() {
            enemyPath = std::make_shared<sf::VertexArray>(sf::VertexArray());
            enemyPath->setPrimitiveType(sf::PrimitiveType::LineStrip);

            sf::Vertex vertices[] {
                {{  0.0f,   430.0f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  800.0f, 430.0f}, sf::Color::Red, { 0.0f, 10.0f}},
                {{800.0f, 200.0f}, sf::Color::Red, {10.0f, 10.0f}},
                {{  540.0f,   200.0f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{540.0f, 800.0f}, sf::Color::Red, {10.0f, 10.0f}},
                {{280.0f,   800.0f}, sf::Color::Red, {10.0f,  0.0f}},
                {{280.0f,   600.0f}, sf::Color::Red, {10.0f,  0.0f}},
                {{1000.0f,   600.0f}, sf::Color::Red, {10.0f,  0.0f}},
                {{1000.0f,   350.0f}, sf::Color::Red, {10.0f,  0.0f}},
                {{1200.0f,   350.0f}, sf::Color::Red, {10.0f,  0.0f}},
                {{1200.0f,   730.0f}, sf::Color::Red, {10.0f,  0.0f}},
                {{730.0f,   730.0f}, sf::Color::Red, {10.0f,  0.0f}},
                {{730.0f,   1080.0f}, sf::Color::Red, {10.0f,  0.0f}}
            };

            for (const auto & vertice : vertices) {
                enemyPath->append(vertice);
            }

            /*
            const auto testEnemy1 = new DevEnemy(enemyPath, 2, 1, sf::Color::Green, 50);
            const auto testEnemy2 = new DevEnemy(enemyPath, 4, 2, sf::Color::Blue, 54);
            const auto testEnemy3 = new DevEnemy(enemyPath, 6, 3, sf::Color(224, 58, 164), 60);
            const auto testEnemy4 = new DevEnemy(enemyPath, 8, 10, sf::Color(92, 30, 11), 75);
            enemies.push_back(testEnemy1);
            enemies.push_back(testEnemy2);
            enemies.push_back(testEnemy3);
            enemies.push_back(testEnemy4);
            */
            const auto testLargeEnemy = new LargeDevEnemy(enemyPath);
            enemies.push_back(testLargeEnemy);
        }

        void update() const {
            for (const auto &enemy : enemies) {
                if (enemy->isAlive()) {
                    enemy->updatePosition();
                }
            }
        }

        void addEnemy(Enemy* newEnemy) {
            enemies.push_back(newEnemy);
            undrawnEnemies.push_back(newEnemy);
        }

        void addEnemies(const std::vector<Enemy*>& newEnemies) {
            for (Enemy* enemy : newEnemies) {
                enemies.push_back(enemy);
                undrawnEnemies.push_back(enemy);
            }
        }

        [[nodiscard]] std::vector<Enemy*> getAliveEnemies() const {
            std::vector<Enemy*> alive;
            for (auto& enemy : enemies) {
                if (enemy->isAlive()) {
                    alive.push_back(enemy);
                }
            }
            return alive;
        }

        [[nodiscard]] std::vector<Enemy*> getUndrawnEnemies() {
            std::vector<Enemy*> alive;
            for (auto& enemy : undrawnEnemies) {
                if (enemy->isAlive()) {
                    alive.push_back(enemy);
                }
            }
            undrawnEnemies.clear();
            return alive;
        }

        [[nodiscard]] std::vector<Enemy*> getDeadEnemies() const {
            std::vector<Enemy*> dead;
            for (auto& enemy : enemies) {
                if (!enemy->isAlive()) {
                    dead.push_back(enemy);
                }
            }
            return dead;
        }

        void replaceDeadEnemiesWithChildren() {
            auto it = enemies.begin();
            for(int i = 0; i < enemies.size(); i++){
                if (!it.operator*()->isAlive()) {
                    addChildEnemy(it.operator*(), it.operator*()->getChildren());
                    //enemies.remove(it.operator*());
                }
                ++it;
            }
        }

        void removeDeadEnemies() {
            enemies.remove_if([](const Enemy* enemy) {
                return !enemy->isAlive();
            });
            undrawnEnemies.remove_if([](const Enemy* enemy) {
                return !enemy->isAlive();
            });
        }

        [[nodiscard]] std::shared_ptr<sf::VertexArray> getEnemyPath() const {
            return enemyPath;
        }

        // TODO: temp; just for testing atm
        void shrinkEnemyPath() const {
            enemyPath->resize(enemyPath->getVertexCount() - 1);
        }
};



#endif //ENEMYMANAGER_H
