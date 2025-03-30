#ifndef YELLOWENEMY_H
#define YELLOWENEMY_H
#include <memory>

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "../BaseNormalEnemy.h"
#include "../green/GreenEnemy.h"


class YellowEnemy final : public BaseNormalEnemy {
    std::shared_ptr<EnemySpriteInjector> spriteInjector;

    public:
        explicit YellowEnemy(const std::shared_ptr<EnemySpriteInjector>& spriteInjector, const std::shared_ptr<sf::VertexArray>& pathToFollow) :
            BaseNormalEnemy(spriteInjector->createYellowAnimHitTexture(), pathToFollow, 4, sf::Color::Yellow, 45.f) {
            this->spriteInjector = spriteInjector;
        }

        std::vector<std::shared_ptr<Enemy>> getChildren() override {
            std::vector<std::shared_ptr<Enemy>> children;
            const auto enemy = std::make_shared<GreenEnemy>(GreenEnemy(spriteInjector, getPathToFollow()));
            children.push_back(enemy);
            return children;
        }

    protected:
        std::shared_ptr<Enemy> copy() override {
            return std::make_shared<YellowEnemy>(*this);
        }
};



#endif //YELLOWENEMY_H
