#ifndef MIDDEVENEMY_H
#define MIDDEVENEMY_H
#include "DevEnemy.h"
#include "SmallDevEnemy.h"


class MidDevEnemy final : public DevEnemy {

    public:
        explicit MidDevEnemy(const std::shared_ptr<sf::VertexArray>& pathToFollow) :
            DevEnemy(pathToFollow, 2, 1, sf::Color::Blue, 40.f) {
        }

        std::vector<std::shared_ptr<Enemy>> getChildren() override {
            std::vector<std::shared_ptr<Enemy>> children;
            const auto enemy = std::make_shared<SmallDevEnemy>(SmallDevEnemy(getPathToFollow()));
            children.push_back(enemy);
            return children;
        }
};



#endif //MIDDEVENEMY_H
