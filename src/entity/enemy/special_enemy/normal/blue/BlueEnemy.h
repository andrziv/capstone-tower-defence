#ifndef BLUEENEMY_H
#define BLUEENEMY_H
#include <memory>

#include "../BaseNormalEnemy.h"
#include "../red/RedEnemy.h"
#include "SFML/Graphics/VertexArray.hpp"


class BaseNormalEnemy;

class BlueEnemy final : public BaseNormalEnemy {
    public:
        explicit BlueEnemy(const std::shared_ptr<sf::VertexArray>& pathToFollow) :
            BaseNormalEnemy(pathToFollow, 2, sf::Color::Blue, 35.f) {
        }

        std::vector<std::shared_ptr<Enemy>> getChildren() override {
            std::vector<std::shared_ptr<Enemy>> children;
            const auto enemy = std::make_shared<RedEnemy>(RedEnemy(getPathToFollow()));
            children.push_back(enemy);
            return children;
        }

    protected:
        std::shared_ptr<Enemy> copy() override {
            return std::make_shared<BlueEnemy>(*this);
        }
};



#endif //BLUEENEMY_H
