#ifndef YELLOWENEMY_H
#define YELLOWENEMY_H
#include <memory>

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "../BaseNormalEnemy.h"
#include "../green/GreenEnemy.h"


class YellowEnemy final : public BaseNormalEnemy {
    public:
        explicit YellowEnemy(const std::shared_ptr<sf::VertexArray>& pathToFollow) :
            BaseNormalEnemy(pathToFollow, 4, sf::Color::Yellow, 45.f) {
        }

        std::vector<std::shared_ptr<Enemy>> getChildren() override {
            std::vector<std::shared_ptr<Enemy>> children;
            const auto enemy = std::make_shared<GreenEnemy>(GreenEnemy(getPathToFollow()));
            children.push_back(enemy);
            return children;
        }
};



#endif //YELLOWENEMY_H
