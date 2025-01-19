#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H
#include <list>

#include "Enemy.h"
#include "special_enemy/dev/DevEnemy.h"
#include "special_enemy/dev/LargeDevEnemy.h"


class EnemyManager {
    std::list<std::shared_ptr<Enemy>> enemies;
    std::list<std::shared_ptr<Enemy>> undrawnEnemies;
    std::shared_ptr<sf::VertexArray> enemyPath;

    void addChildEnemy(const std::shared_ptr<Enemy>& parentEnemy, const std::vector<std::shared_ptr<Enemy>>& childEnemy) {
        for (const std::shared_ptr<Enemy> & child : childEnemy) {
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

            const auto testLargeEnemy = std::make_shared<LargeDevEnemy>(LargeDevEnemy(enemyPath));
            enemies.push_back(testLargeEnemy);
        }

        void update() const {
            for (const auto &enemy : enemies) {
                if (enemy->isAlive()) {
                    enemy->updatePosition();
                }
            }
        }

        void addEnemy(const std::shared_ptr<Enemy>& newEnemy) {
            enemies.push_back(newEnemy);
            undrawnEnemies.push_back(newEnemy);
        }

        void addEnemies(const std::vector<std::shared_ptr<Enemy>>& newEnemies) {
            for (const auto& enemy : newEnemies) {
                enemy->initialize(enemyPath);
                enemies.push_back(enemy);
                undrawnEnemies.push_back(enemy);

            }
        }

        [[nodiscard]] std::vector<std::shared_ptr<Enemy>> getAliveEnemies() const {
            std::vector<std::shared_ptr<Enemy>> alive;
            for (auto& enemy : enemies) {
                if (enemy->isAlive()) {
                    alive.push_back(enemy);
                }
            }
            return alive;
        }

        [[nodiscard]] std::vector<std::shared_ptr<Enemy>> getUndrawnEnemies() {
            std::vector<std::shared_ptr<Enemy>> alive;
            for (auto& enemy : undrawnEnemies) {
                if (enemy->isAlive()) {
                    alive.push_back(enemy);
                }
            }
            undrawnEnemies.clear();
            return alive;
        }

        [[nodiscard]] std::vector<std::shared_ptr<Enemy>> getDeadEnemies() const {
            std::vector<std::shared_ptr<Enemy>> dead;
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
            enemies.remove_if([](const std::shared_ptr<Enemy>& enemy) {
                return !enemy->isAlive();
            });
            undrawnEnemies.remove_if([](const std::shared_ptr<Enemy>& enemy) {
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
