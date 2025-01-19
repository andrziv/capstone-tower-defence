#ifndef GREENENEMY_H
#define GREENENEMY_H
#include <memory>

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "../BaseNormalEnemy.h"
#include "../blue/BlueEnemy.h"

class GreenEnemy final : public BaseNormalEnemy {
    public:
        explicit GreenEnemy(const std::shared_ptr<sf::VertexArray>& pathToFollow) :
            BaseNormalEnemy(pathToFollow, 3, sf::Color::Green, 40.f) {
        }

        std::vector<std::shared_ptr<Enemy>> getChildren() override {
            std::vector<std::shared_ptr<Enemy>> children;
            const auto enemy = std::make_shared<BlueEnemy>(BlueEnemy(getPathToFollow()));
            children.push_back(enemy);
            return children;
        }
};



#endif //GREENENEMY_H
