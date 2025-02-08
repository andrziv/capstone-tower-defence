#ifndef BASENORMALPROJECTILE_H
#define BASENORMALPROJECTILE_H
#include <memory>

#include "../Projectile.h"
#include "SFML/Graphics/Color.hpp"

class BaseNormalProjectile : public Projectile {

public:
    BaseNormalProjectile(const float posX, const float posY, const float direction,
        const int pierce, const int damage, const int speed,
        const float size, const sf::Color color) : Projectile(pierce, damage, speed, posX, posY, direction) {
        getHitTexture()->setDisplayEntityColor(color);
        getHitTexture()->setDisplayEntityRadius(size);
        getHitTexture()->setHitboxColor(sf::Color::Transparent);
        getHitTexture()->setHitboxRadius(size);
        setAngle(direction);
    }

protected:
    void collidedWith(const std::shared_ptr<Enemy> &collided) override {
        collided->subtractHealth(getDamage());
    }
};



#endif //BASENORMALPROJECTILE_H
