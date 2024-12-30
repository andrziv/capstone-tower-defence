#ifndef DEVPROJECTILE_H
#define DEVPROJECTILE_H

#include "../../Projectile.h"
#include "../../../../enemy/Enemy.h"

class DevProjectile final : public Projectile{

    public:
        ~DevProjectile() override = default;

        DevProjectile(const int pierce, const int damage, const int speed, const float posX, const float posY) : Projectile(pierce, damage, speed, posX, posY, 0.f) {
            getHitTexture()->getDisplayEntity()->setFillColor(sf::Color::Yellow);
            getHitTexture()->getDisplayEntity()->setRadius(2.f);
            getHitTexture()->getHitbox()->setFillColor(sf::Color::Transparent);
            getHitTexture()->getHitbox()->setRadius(2.f);
        }

    protected:
        void collidedWith(Enemy* collided) override {
            collided->subtractHealth(getDamage());
        }
};



#endif //DEVPROJECTILE_H
