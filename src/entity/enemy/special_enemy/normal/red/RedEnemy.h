#ifndef REDENEMY_H
#define REDENEMY_H
#include "../BaseNormalEnemy.h"


class RedEnemy final : public BaseNormalEnemy {

    public:
        explicit RedEnemy(const std::shared_ptr<sf::VertexArray>& pathToFollow) :
            BaseNormalEnemy(pathToFollow, 1, sf::Color::Red, 30.f) {
        }
};



#endif //REDENEMY_H
