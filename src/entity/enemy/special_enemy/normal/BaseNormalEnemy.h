#ifndef BASENORMALENEMY_H
#define BASENORMALENEMY_H
#include <memory>

#include "../../Enemy.h"
#include "SFML/Graphics/VertexArray.hpp"


class DevEnemy;

class BaseNormalEnemy : public Enemy {
    public:
        ~BaseNormalEnemy() override = default;

        BaseNormalEnemy(const std::shared_ptr<sf::VertexArray>& pathToFollow, const float speed, const sf::Color color, const float size) : Enemy(pathToFollow, speed, 1) {
            getHitTexture()->setDisplayEntityColor(color);
            getHitTexture()->setDisplayEntityRadius(size);
            getHitTexture()->setHitboxColor(sf::Color::Transparent);
            getHitTexture()->setHitboxRadius(size);
        }
};



#endif //BASENORMALENEMY_H
