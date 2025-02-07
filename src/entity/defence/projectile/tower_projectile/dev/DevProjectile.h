#ifndef DEVPROJECTILE_H
#define DEVPROJECTILE_H

#include "../../Projectile.h"
#include "../../../../enemy/Enemy.h"
#include "SFML/Audio/Listener.hpp"

class DevProjectile final : public Projectile {

    public:
        ~DevProjectile() override = default;

        DevProjectile(const int pierce, const int damage, const int speed, const float posX, const float posY) : Projectile(pierce, damage, speed, posX, posY, 0.f) {
            getHitTexture()->setDisplayEntityColor(sf::Color::Yellow);
            getHitTexture()->setDisplayEntityRadius(2.f);
            getHitTexture()->setHitboxColor(sf::Color::Transparent);
            getHitTexture()->setHitboxRadius(2.f);
        }

        DevProjectile(const int pierce, const int damage, const int speed, const float posX, const float posY, const sf::Color color) : DevProjectile(pierce, damage, speed, posX, posY) {
            getHitTexture()->getDisplayEntity()->setFillColor(color);
        }

        DevProjectile(const int pierce, const int damage, const int speed, const float posX, const float posY, const sf::Color color, const float size) : DevProjectile(pierce, damage, speed, posX, posY, color) {
            getHitTexture()->setDisplayEntityRadius(size);
            getHitTexture()->setHitboxRadius(size);
        }

        DevProjectile(const int pierce, const int damage, const int speed, const float posX, const float posY, const sf::Color color, const float size, const float direction) : DevProjectile(pierce, damage, speed, posX, posY, color, size) {
            getHitTexture()->setDisplayEntityRadius(size);
            getHitTexture()->setHitboxRadius(size);
            setAngle(direction);
        }

    protected:
        void collidedWith(const std::shared_ptr<Enemy> &collided) override {
            collided->subtractHealth(getDamage());
        }
};



#endif //DEVPROJECTILE_H
