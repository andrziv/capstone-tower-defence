#ifndef BALLOON_H
#define BALLOON_H
#include <algorithm>
#include <cmath>
#include <string>

#include "../../helper/UUID.h"
#include "../hit_texture/circle/CircleHitTexture.h"
#include "SFML/Graphics/Vertex.hpp"


namespace sf {
    class VertexArray;
}

class Enemy {
    std::string id = get_uuid();
    sf::Vertex position;
    std::shared_ptr<sf::VertexArray> path;
    int currentNodeTarget = 0;
    std::shared_ptr<CircleHitTexture> hitTexture;
    int health;
    double speed;
    int reward = 0;

    void initialize() {
        if (this->path != nullptr && this->path->getVertexCount() > 0) {
            this->position.position.x = path->operator[](0).position.x;
            this->position.position.y = path->operator[](0).position.y;
            this->hitTexture->setPosition(this->position.position);
        }
    }

    void setHitTexture(const std::shared_ptr<CircleHitTexture>& hitTexture) {
        this->hitTexture = hitTexture;
    }

    public:
        virtual ~Enemy() = default;

        Enemy(const std::shared_ptr<sf::VertexArray>& pathToFollow, const float speed, const int health, const int reward) {
            this->hitTexture = std::make_shared<CircleHitTexture>(CircleHitTexture());
            this->path = pathToFollow;
            if (path != nullptr) {
                initialize();
            }
            this->speed = speed;
            this->health = health;
            this->reward = reward;
        }

        void initialize(const std::shared_ptr<sf::VertexArray>& pathToFollow) {
            this->path = pathToFollow;
            initialize();
        }

        void updatePosition() {
            if (path == nullptr) {
                return;
            }
            if (path->getVertexCount() > 0 && currentNodeTarget < path->getVertexCount()) {
                double distanceYetToTravel = speed;
                while (distanceYetToTravel > 0 && path->getVertexCount() - currentNodeTarget > 0) {
                    const auto nextNode = path->operator[](currentNodeTarget).position;
                    double distanceToNextNode = std::abs(sqrt(pow(nextNode.x - position.position.x, 2) + pow(nextNode.y - position.position.y, 2)));
                    distanceYetToTravel -= std::max(static_cast<double>(0), distanceToNextNode);
                    distanceYetToTravel = std::max(static_cast<double>(0), distanceYetToTravel);
                    const auto distanceToTravel = std::min(speed, distanceToNextNode);
                    if (std::abs(nextNode.y - position.position.y) + std::abs(nextNode.x - position.position.x) != 0) {
                        auto xRatio = (nextNode.x - position.position.x) / (std::abs(nextNode.y - position.position.y) + std::abs(nextNode.x - position.position.x));
                        auto yRatio = (nextNode.y - position.position.y) / (std::abs(nextNode.y - position.position.y) + std::abs(nextNode.x - position.position.x));
                        position.position.x += xRatio * static_cast<float>(distanceToTravel);
                        position.position.y += yRatio * static_cast<float>(distanceToTravel);
                        getHitTexture()->setPosition(position.position.x, position.position.y);
                    }
                    if (distanceToNextNode <= 0) {
                        currentNodeTarget++;
                    }
                }
            }
        }

        [[nodiscard]] sf::Vertex getPosition() const {
            return position;
        }

        void setPosition(const sf::Vector2f newPos) {
            position.position = newPos;
            hitTexture->setPosition(newPos);
        }

        [[nodiscard]] int getTargetNode() const {
            return currentNodeTarget;
        }

        void setTargetNode(const int newTarget) {
            currentNodeTarget = newTarget;
        }

        std::shared_ptr<CircleHitTexture> getHitTexture() {
            return hitTexture;
        }

        void subtractHealth(const int toSubtract) {
            health -= toSubtract;
        }

        void kill() {
            health = 0;
        }

        [[nodiscard]] bool isAlive() const {
            return health > 0;
        }

        virtual std::vector<std::shared_ptr<Enemy>> getChildren() { return {}; }

        std::string getId() {
            return id;
        }

        void setId(const std::string &newId) {
            id = newId;
        }

        [[nodiscard]] int getReward() const {
            return reward;
        }

        bool operator == (const Enemy &other) const {
            if(this->id == other.id) {
                return true;
            }
            return false;
        }

        std::shared_ptr<Enemy> deep_copy() {
            const auto copiedHitTexture = std::make_shared<CircleHitTexture>(*getHitTexture());
            const auto copiedDisplayEntity = std::make_shared<sf::CircleShape>(*getHitTexture()->getCircleDisplayEntity());
            const auto copiedHitbox = std::make_shared<sf::CircleShape>(*getHitTexture()->getCircleHitbox());
            auto copiedEnemy = copy();
            copiedHitTexture->setDisplayEntity(copiedDisplayEntity);
            copiedHitTexture->setHitbox(copiedHitbox);
            copiedEnemy->setHitTexture(copiedHitTexture);
            copiedEnemy->setId(get_uuid());
            return copiedEnemy;
        }

    protected:
        virtual std::shared_ptr<Enemy> copy() = 0;

        [[nodiscard]] std::shared_ptr<sf::VertexArray> getPathToFollow() const {
            return path;
        }
};



#endif //BALLOON_H
