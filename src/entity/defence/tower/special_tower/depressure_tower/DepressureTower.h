#ifndef DEPRESSURETOWER_H
#define DEPRESSURETOWER_H
#include <memory>
#include <vector>

#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
#include "../BaseTower.h"
#include "../../Tower.h"
#include "../../../projectile/tower_projectile/basic/BasicProjectile.h"

class Projectile;

class DepressureTower final : public BaseTower {

    bool alreadyIncreasedCore = false;

    public:
        explicit DepressureTower(const sf::Vector2f& position)
            : BaseTower (position, 0, 50, 0.f, 0, "Depressure", sf::Color::Magenta)  {
        }

        std::vector<std::shared_ptr<Projectile>> shootProjectile(std::vector<std::shared_ptr<Enemy>>& enemies) override {
            if (!alreadyIncreasedCore) {
                setActiveCoresTo(activeCores + 1);
                alreadyIncreasedCore = true;
            }
            return {};
        }

    protected:
        std::shared_ptr<Tower> copy() override {
            return std::make_shared<DepressureTower>(*this);
        }

        std::vector<std::shared_ptr<Projectile>> generateProjectiles(const double angle) override {
            return {};
        }

        std::string getPressurePattern() override {
            return R"~(0)~";
        }
};



#endif //DEPRESSURETOWER_H
