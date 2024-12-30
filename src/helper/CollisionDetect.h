//
// Created by aki on 12/29/24.
//

#ifndef COLLISIONDETECT_H
#define COLLISIONDETECT_H

#include <cmath>
#include "SFML/Graphics/CircleShape.hpp"

inline bool doCirclesOverlap(const sf::CircleShape& circle1, const sf::CircleShape& circle2) {
    const double r1 = circle1.getRadius();
    const double x1 = circle1.getPosition().x - r1;
    const double y1 = circle1.getPosition().y - r1;
    const double r2 = circle2.getRadius();
    const double x2 = circle2.getPosition().x - r2;
    const double y2 = circle2.getPosition().y - r2;

    const double d = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

    if (d <= r1 - r2) {
        return true;
    }
    if (d <= r2 - r1) {
        return true;
    }
    if (d < r1 + r2) {
        return true;
    }
    if (d == r1 + r2) {
        return true;
    }
    return false;
}

#endif //COLLISIONDETECT_H
