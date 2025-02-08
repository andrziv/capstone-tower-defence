#ifndef BOMBPROJECTILE_H
#define BOMBPROJECTILE_H
#include "../BaseExplosiveProjectile.h"


class BombProjectile final : public BaseExplosiveProjectile {
public:
    BombProjectile(const float posX, const float posY, const float direction)
        : BaseExplosiveProjectile(posX, posY, direction, 10, 150.f, 8.f) {
    }
};



#endif //BOMBPROJECTILE_H
