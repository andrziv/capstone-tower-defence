#ifndef PROJECTILE_H
#define PROJECTILE_H
#include <algorithm>
#include <complex>
#include <vector>

#include "SFML/System/Vector2.hpp"

#include "../../enemy/Enemy.h"
#include "../../HitTexture.h"
#include "../../../helper/UUID.h"
#include "../../../helper/CollisionDetect.h"

class Projectile {
    std::string id = get_uuid();
    int pierce;
    int damage;
    int speed;
    float angle;
    sf::Vector2f pos;
    HitTexture hitTexture;
    std::vector<Enemy*> collisions;

    Projectile(const int pierce, const int damage, const int speed, const HitTexture &hitTexture): angle(0) {
        this->pierce = pierce;
        this->damage = damage;
        this->speed = speed;
        this->hitTexture = hitTexture;
    }

public:
    virtual ~Projectile() {}

    Projectile(const int pierce, const int damage, const int speed, const float posX, const float posY, const float direction, const HitTexture &hitTexture) : Projectile(pierce, damage, speed, hitTexture) {
        this->pos.x = posX;
        this->pos.y = posY;
        this->angle = direction;
        this->hitTexture.setPosition(pos);
    }

    void updatePosition() {
        pos.x += this->speed * cos(angle);
        pos.y += this->speed * sin(angle);
        hitTexture.setPosition(pos);
    }

    void onCollision(Enemy* collided) {
        if (!alreadyCollided(collided)) {
            collisions.push_back(collided);
            collidedWith(collided);
            pierce--;
        }
    }

    bool isColliding(Enemy *toCheck) {
        return doCirclesOverlap(*hitTexture.getHitbox(), *toCheck->getHitTexture()->getHitbox());
    }

    HitTexture getHitTexture() {
        return hitTexture;
    }

    bool isValid() const {
        return pierce > 0;
    }

    bool operator == (const Projectile &other) const {
        if(this->id == other.id) {
            return true;
        }
        return false;
    }

protected:
    bool alreadyCollided(const Enemy* collided) {
        return std::find(collisions.begin(), collisions.end(), collided) != collisions.end();
    }

    virtual void collidedWith(Enemy* collided) {}

    void setPos(const sf::Vector2f& pos) {
        this->pos = pos;
    }

    [[nodiscard]] sf::Vector2f getPos() const {
        return pos;
    }

    [[nodiscard]] int getDamage() const {
        return damage;
    }

    void setPierce(const int pierce) {
        this->pierce = pierce;
    }

    [[nodiscard]] int getPierce() const {
        return pierce;
    }
};



#endif //PROJECTILE_H
