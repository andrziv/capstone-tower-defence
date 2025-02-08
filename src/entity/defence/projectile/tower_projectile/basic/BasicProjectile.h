#ifndef BASICPROJECTILE_H
#define BASICPROJECTILE_H
#include "../BaseNormalProjectile.h"
#include "../../Projectile.h"


class BasicProjectile final : public BaseNormalProjectile {

public:
    BasicProjectile(const float posX, const float posY, const float direction, const int pierce)
        : BaseNormalProjectile(posX, posY, direction, pierce, 1, 20, 4.f, sf::Color(180, 180, 180)) {
    }

    BasicProjectile(const float posX, const float posY, const float direction)
        : BasicProjectile(posX, posY, direction, 1) {
    }
};



#endif //BASICPROJECTILE_H
