#ifndef BALLOON_H
#define BALLOON_H
#include <algorithm>
#include <cmath>
#include <utility>

#include "SFML/Graphics/Vertex.hpp"
#include "SFML/Graphics/VertexArray.hpp"

class Enemy
{
    sf::Vertex position;
    sf::VertexArray *path;
    int currentNodeTarget = 0;
    double speed;

    public:
        Enemy(sf::VertexArray *pathToFollow, const float speed) {
            this->path = pathToFollow;
            if (this->path->getVertexCount() > 0) {
                this->position.position.x = path->operator[](0).position.x;
                this->position.position.y = path->operator[](0).position.y;
            }
            this->speed = speed;
        }

        void update() {
            if (path->getVertexCount() > 0 && currentNodeTarget < path->getVertexCount()) {
                double distanceYetToTravel = speed;
                while (distanceYetToTravel > 0 && path->getVertexCount() - currentNodeTarget > 0) {
                    const auto nextNode = path->operator[](currentNodeTarget).position;
                    double distanceToNextNode = std::abs(sqrt(pow(nextNode.x - position.position.x, 2) + pow(nextNode.y - position.position.y, 2)));
                    distanceYetToTravel -= std::max(static_cast<double>(0), distanceToNextNode);
                    distanceYetToTravel = std::max(static_cast<double>(0), distanceYetToTravel);
                    const auto distanceToTravel = std::min(speed, distanceToNextNode);
                    if ((std::abs(nextNode.y - position.position.y) + std::abs(nextNode.x - position.position.x)) != 0) {
                        double xRatio = (nextNode.x - position.position.x) / (std::abs(nextNode.y - position.position.y) + std::abs(nextNode.x - position.position.x));
                        double yRatio = (nextNode.y - position.position.y) / (std::abs(nextNode.y - position.position.y) + std::abs(nextNode.x - position.position.x));
                        position.position.x += xRatio * distanceToTravel;
                        position.position.y += yRatio * distanceToTravel;
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
};



#endif //BALLOON_H
