#ifndef MIDDEVENEMY_H
#define MIDDEVENEMY_H
#include "DevEnemy.h"
#include "SmallDevEnemy.h"


class MidDevEnemy final : public DevEnemy {

    public:
        explicit MidDevEnemy(const std::shared_ptr<sf::VertexArray>& pathToFollow) :
            DevEnemy(pathToFollow, 2, 1, sf::Color::Blue, 40.f) {
        }

        std::vector<Enemy*> getChildren() override {
            std::vector<Enemy*> children;
            Enemy* enemy = new SmallDevEnemy(getPathToFollow());
            enemy->setPosition(getPosition().position);
            enemy->setTargetNode(getTargetNode());
            enemy->setId(getId());
            children.push_back(enemy);
            return children;
        }
};



#endif //MIDDEVENEMY_H
