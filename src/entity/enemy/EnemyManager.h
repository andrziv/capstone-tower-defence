#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H
#include <list>

#include "Enemy.h"
#include "../../helper/visual/ThickLine.h"

struct EnemySpawn {
    std::string spawnGap;
    std::vector<std::shared_ptr<Enemy>> enemiesToSpawn;
};

class EnemyManager {
    // TODO: temp texture location till we find a better way to circumvent the x11 error for slave nodes
    sf::Texture BUBBLE_TEXTURE = sf::Texture("../../src/resources/textures/bubble.png");
    sf::Texture WARN_TEXTURE = sf::Texture("../../src/resources/textures/warn.png");

    std::list<std::shared_ptr<Enemy>> enemies;
    std::list<std::shared_ptr<Enemy>> undrawnEnemies;
    std::shared_ptr<sf::VertexArray> enemyPath;
    std::shared_ptr<sf::VertexArray> visualEnemyPath;

    void addChildEnemy(const std::shared_ptr<Enemy>& parentEnemy, const std::vector<std::shared_ptr<Enemy>>& childEnemy) {
        for (const std::shared_ptr<Enemy> & child : childEnemy) {
            child->setPosition(parentEnemy->getPosition().position);
            child->setTargetNode(parentEnemy->getTargetNode());
            child->setId(parentEnemy->getId());
            addEnemy(child);
        }
    }

    static float calculateSpawnGapSize(const std::string& spawnGapType, const std::shared_ptr<Enemy>& enemy) {
        const auto enemyHitBoxRadius = enemy->getHitTexture()->getCircleHitbox()->getRadius();
        if (spawnGapType == "FAR") {
            return enemyHitBoxRadius * 2 + 20;
        }
        if (spawnGapType == "SIDEBY") {
            return enemyHitBoxRadius * 2;
        }
        if (spawnGapType == "CLOSE") {
            return enemyHitBoxRadius / 2;
        }
        return enemyHitBoxRadius;
    }

    public:
        // TODO: temp; just for testing atm
        EnemyManager() {
            enemyPath = std::make_shared<sf::VertexArray>(sf::VertexArray());
            enemyPath->setPrimitiveType(sf::PrimitiveType::LineStrip);

            sf::Vertex vertices[] {
                {{  3.000000f,   453.000000f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  905.000000f,   451.000000f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  952.000000f,   399.000000f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  951.000000f,   247.000000f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  891.000000f,   203.000000f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  681.000000f,   203.000000f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  626.000000f,   240.000000f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  625.000000f,   805.000000f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  559.000000f,   875.000000f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  379.000000f,   877.000000f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  312.000000f,   816.000000f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  311.000000f,   677.000000f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  358.000000f,   632.000000f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  1129.000000f,   617.000000f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  1210.000000f,   570.000000f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  1211.000000f,   413.000000f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  1265.000000f,   351.000000f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  1400.000000f,   353.000000f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  1453.000000f,   415.000000f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  1448.000000f,   714.000000f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  1403.000000f,   774.000000f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  926.000000f,   788.000000f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  856.000000f,   831.000000f}, sf::Color::Red, { 0.0f,  0.0f}},
                {{  849.000000f,   1078.000000f}, sf::Color::Red, { 0.0f,  0.0f}}
            };

            std::vector<sf::Vector2f> points;

            for (const auto & vertice : vertices) {
                enemyPath->append(vertice);
                points.push_back(vertice.position);
            }
            ThickLine thickLine(points, 10.f, sf::Color(102,102,102));
            visualEnemyPath = std::make_shared<sf::VertexArray>(thickLine.getShape());
        }

        void update() const {
            for (const auto &enemy : enemies) {
                if (enemy->isAlive()) {
                    enemy->updatePosition();
                }
            }
        }

        void addEnemy(const std::shared_ptr<Enemy>& newEnemy) {
            //TODO: temp
            newEnemy->getHitTexture()->setTexture(WARN_TEXTURE);

            enemies.push_back(newEnemy);
            undrawnEnemies.push_back(newEnemy);
        }

        void addEnemies(const std::vector<std::shared_ptr<Enemy>>& newEnemies) {
            for (const auto& enemy : newEnemies) {
                //TODO: temp
                enemy->getHitTexture()->setTexture(WARN_TEXTURE);

                enemy->initialize(enemyPath);
                enemies.push_back(enemy);
                undrawnEnemies.push_back(enemy);
            }
        }

        void addEnemies(const std::vector<EnemySpawn>& newSpawnGroup) {
            for (const auto&[spawnGap, enemiesToSpawn] : newSpawnGroup) {
                int counter = 0;
                for (const auto& enemy : enemiesToSpawn) {
                    //TODO: TEMP
                    enemy->getHitTexture()->setTexture(WARN_TEXTURE);

                    const auto spawnGapDistance = calculateSpawnGapSize(spawnGap, enemy);
                    enemy->initialize(enemyPath);
                    const sf::Vertex startPosition = enemy->getPosition();
                    enemy->setPosition(sf::Vector2f(startPosition.position.x - spawnGapDistance * static_cast<float>(counter), startPosition.position.y));
                    enemies.push_back(enemy);
                    undrawnEnemies.push_back(enemy);
                    counter++;
                }
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

        [[nodiscard]] std::vector<std::shared_ptr<Enemy>> getEnemiesAtEndOfPath() const {
            std::vector<std::shared_ptr<Enemy>> dead;
            for (auto& enemy : enemies) {
                if (enemy->getTargetNode() >= enemyPath->getVertexCount()) {
                    enemy->kill();
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

        [[nodiscard]] std::shared_ptr<sf::VertexArray> getVisualEnemyPath() const {
            return visualEnemyPath;
        }

        [[nodiscard]] unsigned long getNumberOfAliveEnemies() const {
            return enemies.size();
        }

        // TODO: temp; just for testing atm
        void shrinkEnemyPath() const {
            if (enemyPath->getVertexCount() - 1 > 0) {
                enemyPath->resize(enemyPath->getVertexCount() - 1);
                visualEnemyPath->resize(visualEnemyPath->getVertexCount() - 2);
            }
        }
};



#endif //ENEMYMANAGER_H
