#ifndef COLLISIONDETECT_H
#define COLLISIONDETECT_H

#include <cmath>
#include "SFML/Graphics/CircleShape.hpp"

inline bool doCirclesOverlap(const sf::CircleShape& circle1, const sf::CircleShape& circle2) {
    const double r1 = circle1.getRadius();
    const double x1 = circle1.getPosition().x;
    const double y1 = circle1.getPosition().y;
    const double r2 = circle2.getRadius();
    const double x2 = circle2.getPosition().x;
    const double y2 = circle2.getPosition().y;

    // get distance between the circle's centers
    // use the Pythagorean Theorem to compute the distance
    const float distX = x1 - x2;
    const float distY = y1 - y2;

    // if the distance is less than the sum of the circle's
    // radii, the circles are touching!
    if (const float distance = sqrt((distX * distX) + (distY * distY)); distance <= r1 + r2) {
        return true;
    }
    return false;
}

#endif //COLLISIONDETECT_H
