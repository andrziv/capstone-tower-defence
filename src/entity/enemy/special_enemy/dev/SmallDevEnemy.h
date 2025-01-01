#ifndef SMALLDEVENEMY_H
#define SMALLDEVENEMY_H
#include "DevEnemy.h"


class SmallDevEnemy final : public DevEnemy {

    public:
        explicit SmallDevEnemy(const std::shared_ptr<sf::VertexArray>& pathToFollow) :
            DevEnemy(pathToFollow, 1, 1, sf::Color::Red, 30.f) {
        }
};



#endif //SMALLDEVENEMY_H
