#ifndef DEVENEMY_H
#define DEVENEMY_H

#include "../../Enemy.h"
#include "../../../HitTexture.h"

class DevEnemy final : public Enemy{

    public:
        ~DevEnemy() override = default;

        DevEnemy(sf::VertexArray *pathToFollow, const float speed, const int health, const sf::Color color, const float size) : Enemy(pathToFollow, speed, health) {
            getHitTexture()->getDisplayEntity()->setFillColor(color);
            getHitTexture()->getDisplayEntity()->setRadius(size);
            getHitTexture()->getHitbox()->setFillColor(sf::Color::Transparent);
            getHitTexture()->getHitbox()->setRadius(size);
        }
};



#endif //DEVENEMY_H
