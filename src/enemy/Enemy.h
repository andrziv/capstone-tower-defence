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
    sf::VertexArray path;
    double speed;

    void removeFirstPathVertex() {
        for (int i = 0; i < path.getVertexCount(); i++) {
            if (i != path.getVertexCount() - 1) {
                path.operator[](i) = path.operator[](i + 1);
            }
        }
        path.resize(path.getVertexCount() - 1);
    }

    public:
        Enemy(sf::VertexArray pathToFollow, float speed) {
            this->path = std::move(pathToFollow);
            this->speed = speed;
        }

        void update() {
            if (path.getVertexCount() > 0) {
                double distanceYetToTravel = speed;
                while (distanceYetToTravel > 0 && path.getVertexCount() > 0) {
                    const auto nextNode = path.operator[](0).position;
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
                        removeFirstPathVertex();
                    }
                }
            }
        }

        [[nodiscard]] sf::Vertex getPosition() const {
            return position;
        }
};



#endif //BALLOON_H
