#ifndef BALLOON_H
#define BALLOON_H
#include <algorithm>
#include <cmath>
#include <string>

#include "../HitTexture.h"
#include "../../helper/UUID.h"
#include "SFML/Graphics/Vertex.hpp"


namespace sf {
    class VertexArray;
}

class Enemy {
    std::string id = get_uuid();
    sf::Vertex position;
    std::shared_ptr<sf::VertexArray> path;
    int currentNodeTarget = 0;
    HitTexture hitTexture;
    int health;
    double speed;

    public:
        virtual ~Enemy() = default;

        Enemy(const std::shared_ptr<sf::VertexArray>& pathToFollow, const float speed, const int health) {
            this->path = pathToFollow;
            if (this->path->getVertexCount() > 0) {
                this->position.position.x = path->operator[](0).position.x;
                this->position.position.y = path->operator[](0).position.y;
            }
            this->speed = speed;
            this->health = health;
        }

        void updatePosition() {
            if (path->getVertexCount() > 0 && currentNodeTarget < path->getVertexCount()) {
                double distanceYetToTravel = speed;
                while (distanceYetToTravel > 0 && path->getVertexCount() - currentNodeTarget > 0) {
                    const auto nextNode = path->operator[](currentNodeTarget).position;
                    double distanceToNextNode = std::abs(sqrt(pow(nextNode.x - position.position.x, 2) + pow(nextNode.y - position.position.y, 2)));
                    distanceYetToTravel -= std::max(static_cast<double>(0), distanceToNextNode);
                    distanceYetToTravel = std::max(static_cast<double>(0), distanceYetToTravel);
                    const auto distanceToTravel = std::min(speed, distanceToNextNode);
                    if (std::abs(nextNode.y - position.position.y) + std::abs(nextNode.x - position.position.x) != 0) {
                        double xRatio = (nextNode.x - position.position.x) / (std::abs(nextNode.y - position.position.y) + std::abs(nextNode.x - position.position.x));
                        double yRatio = (nextNode.y - position.position.y) / (std::abs(nextNode.y - position.position.y) + std::abs(nextNode.x - position.position.x));
                        position.position.x += xRatio * distanceToTravel;
                        position.position.y += yRatio * distanceToTravel;
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

        HitTexture *getHitTexture() {
            return &hitTexture;
        }

        void subtractHealth(const int toSubtract) {
            health -= toSubtract;
        }

        [[nodiscard]] bool isAlive() const {
            return health > 0;
        }

        bool operator == (const Enemy &other) const {
            if(this->id == other.id) {
                return true;
            }
            return false;
        }
};



#endif //BALLOON_H
