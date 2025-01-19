#ifndef PINKENEMY_H
#define PINKENEMY_H
#include <memory>

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "../BaseNormalEnemy.h"
#include "../yellow/YellowEnemy.h"

class PinkEnemy final : public BaseNormalEnemy {
    public:
        explicit PinkEnemy(const std::shared_ptr<sf::VertexArray>& pathToFollow) :
            BaseNormalEnemy(pathToFollow, 5, sf::Color(255, 182, 193), 50.f) {
        }

        std::vector<std::shared_ptr<Enemy>> getChildren() override {
            std::vector<std::shared_ptr<Enemy>> children;
            const auto enemy = std::make_shared<YellowEnemy>(YellowEnemy(getPathToFollow()));
            children.push_back(enemy);
            return children;
        }
};



#endif //PINKENEMY_H
