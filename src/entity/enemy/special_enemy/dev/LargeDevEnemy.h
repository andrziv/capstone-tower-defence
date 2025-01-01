#ifndef LARGEDEVENEMY_H
#define LARGEDEVENEMY_H
#include "DevEnemy.h"
#include "MidDevEnemy.h"


class LargeDevEnemy final : public DevEnemy {

    public:
        explicit LargeDevEnemy(const std::shared_ptr<sf::VertexArray>& pathToFollow) :
            DevEnemy(pathToFollow, 4, 1, sf::Color::Green, 50.f) {
        }

        std::vector<Enemy*> getChildren() override {
            std::vector<Enemy*> children;
            Enemy* enemy = new MidDevEnemy(getPathToFollow());
            Enemy* enemy2 = new MidDevEnemy(getPathToFollow());
            children.push_back(enemy);
            children.push_back(enemy2);
            return children;
        }
};



#endif //LARGEDEVENEMY_H
