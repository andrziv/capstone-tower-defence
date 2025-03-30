#ifndef GREENENEMY_H
#define GREENENEMY_H
#include <memory>

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "../BaseNormalEnemy.h"
#include "../blue/BlueEnemy.h"

class GreenEnemy final : public BaseNormalEnemy {
    std::shared_ptr<EnemySpriteInjector> spriteInjector;

public:
    explicit GreenEnemy(const std::shared_ptr<EnemySpriteInjector> &spriteInjector,
                        const std::shared_ptr<sf::VertexArray> &pathToFollow) : BaseNormalEnemy(
        spriteInjector->createAnimHitTexture(), spriteInjector->createAnimDeathTexture(), pathToFollow, 3,
        sf::Color::Green, 40.f) {
        this->spriteInjector = spriteInjector;
    }

    std::vector<std::shared_ptr<Enemy> > getChildren() override {
        std::vector<std::shared_ptr<Enemy> > children;
        const auto enemy = std::make_shared<BlueEnemy>(BlueEnemy(spriteInjector, getPathToFollow()));
        children.push_back(enemy);
        return children;
    }

protected:
    std::shared_ptr<Enemy> copy() override {
        return std::make_shared<GreenEnemy>(*this);
    }
};


#endif //GREENENEMY_H
