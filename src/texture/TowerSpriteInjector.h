#ifndef TOWERSPRITEINJECTOR_H
#define TOWERSPRITEINJECTOR_H
#include <memory>

#include "../entity/hit_texture/rectangle/animated_rectangle/AnimRectangleHitTexture.h"
#include "SFML/Graphics/Texture.hpp"


class TowerSpriteInjector {
    std::shared_ptr<sf::Texture> archerMaxTexture;
    std::shared_ptr<sf::Texture> radialMaxTexture;
    std::shared_ptr<sf::Texture> cannonMaxTexture;
    std::shared_ptr<sf::Texture> depressorTexture;

public:
    TowerSpriteInjector() {
        archerMaxTexture = std::make_shared<sf::Texture>(sf::Texture());
        radialMaxTexture = std::make_shared<sf::Texture>(sf::Texture());
        cannonMaxTexture = std::make_shared<sf::Texture>(sf::Texture());
        depressorTexture = std::make_shared<sf::Texture>(sf::Texture());

        if (!archerMaxTexture->loadFromFile("../../src/resources/textures/towers/normal/ARCHER_MAX.png")) {
            return;
        }
        if (!radialMaxTexture->loadFromFile("../../src/resources/textures/towers/normal/ARCHER_MAX.png")) {
            return;
        }
        if (!cannonMaxTexture->loadFromFile("../../src/resources/textures/towers/normal/CANNON_MAGE.png")) {
            return;
        }
        if (!depressorTexture->loadFromFile("../../src/resources/textures/towers/normal/DEPRESS_TOWER.png")) {
            return;
        }
    }

    [[nodiscard]] std::shared_ptr<AnimRectangleHitTexture> createArcherMaxIdleHitTexture() const {
        return std::make_shared<AnimRectangleHitTexture>(AnimRectangleHitTexture(*archerMaxTexture, 70, 130, 6, 0.1f));
    }

    [[nodiscard]] std::shared_ptr<AnimRectangleHitTexture> createRadialMaxIdleHitTexture() const {
        return std::make_shared<AnimRectangleHitTexture>(AnimRectangleHitTexture(*radialMaxTexture, 70, 130, 6, 0.1f));
    }

    [[nodiscard]] std::shared_ptr<AnimRectangleHitTexture> createCannonMaxIdleHitTexture() const {
        return std::make_shared<AnimRectangleHitTexture>(AnimRectangleHitTexture(*cannonMaxTexture, 30, 44, 6, 0.1f));
    }

    [[nodiscard]] std::shared_ptr<AnimRectangleHitTexture> createDepressIdleHitTexture() const {
        return std::make_shared<AnimRectangleHitTexture>(AnimRectangleHitTexture(*depressorTexture, 64, 64, 25, 0.2f));
    }
};



#endif //TOWERSPRITEINJECTOR_H
