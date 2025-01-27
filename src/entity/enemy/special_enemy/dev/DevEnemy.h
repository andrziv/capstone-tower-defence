#ifndef DEVENEMY_H
#define DEVENEMY_H

#include "../../Enemy.h"
#include "../../../hit_texture/HitTexture.h"

class DevEnemy : public Enemy{

    public:
        ~DevEnemy() override = default;

        DevEnemy(const std::shared_ptr<sf::VertexArray>& pathToFollow, const float speed, const int health, const sf::Color color, const float size) : Enemy(pathToFollow, speed, health) {
            getHitTexture()->setDisplayEntityColor(color);
            getHitTexture()->setDisplayEntityRadius(size);
            getHitTexture()->setHitboxColor(sf::Color::Transparent);
            getHitTexture()->setHitboxRadius(size);
        }
};



#endif //DEVENEMY_H
