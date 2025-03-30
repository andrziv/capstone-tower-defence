//
// Created by aki on 3/29/25.
//

#ifndef ENEMYSPRITEINJECTOR_H
#define ENEMYSPRITEINJECTOR_H
#include "../entity/hit_texture/circle/animated_circle/AnimCircleHitTexture.h"
#include "SFML/Graphics/Texture.hpp"


class EnemySpriteInjector {
    std::shared_ptr<sf::Texture> redEnemyTexture;
    std::shared_ptr<sf::Texture> blueEnemyTexture;
    std::shared_ptr<sf::Texture> greenEnemyTexture;
    std::shared_ptr<sf::Texture> yellowEnemyTexture;
    std::shared_ptr<sf::Texture> pinkEnemyTexture;

public:
    EnemySpriteInjector() {
        redEnemyTexture = std::make_shared<sf::Texture>(sf::Texture());
        blueEnemyTexture = std::make_shared<sf::Texture>(sf::Texture());
        greenEnemyTexture = std::make_shared<sf::Texture>(sf::Texture());
        yellowEnemyTexture = std::make_shared<sf::Texture>(sf::Texture());
        pinkEnemyTexture = std::make_shared<sf::Texture>(sf::Texture());

        if (!redEnemyTexture->loadFromFile("../../src/resources/textures/slime/D_Walk.png")) {
            return;
        }
        if (!blueEnemyTexture->loadFromFile("../../src/resources/textures/slime/D_Walk.png")) {
            return;
        }
        if (!greenEnemyTexture->loadFromFile("../../src/resources/textures/slime/D_Walk.png")) {
            return;
        }
        if (!yellowEnemyTexture->loadFromFile("../../src/resources/textures/slime/D_Walk.png")) {
            return;
        }
        if (!pinkEnemyTexture->loadFromFile("../../src/resources/textures/slime/D_Walk.png")) {
            return;
        }
    }

    [[nodiscard]] std::shared_ptr<AnimCircleHitTexture> createRedAnimHitTexture() const {
        return std::make_shared<AnimCircleHitTexture>(AnimCircleHitTexture(*redEnemyTexture, 48, 48, 6, 0.1f));
    }

    [[nodiscard]] std::shared_ptr<AnimCircleHitTexture> createBlueAnimHitTexture() const {
        return std::make_shared<AnimCircleHitTexture>(AnimCircleHitTexture(*redEnemyTexture, 48, 48, 6, 0.1f));
    }

    [[nodiscard]] std::shared_ptr<AnimCircleHitTexture> createGreenAnimHitTexture() const {
        return std::make_shared<AnimCircleHitTexture>(AnimCircleHitTexture(*redEnemyTexture, 48, 48, 6, 0.1f));
    }

    [[nodiscard]] std::shared_ptr<AnimCircleHitTexture> createYellowAnimHitTexture() const {
        return std::make_shared<AnimCircleHitTexture>(AnimCircleHitTexture(*redEnemyTexture, 48, 48, 6, 0.1f));
    }

    [[nodiscard]] std::shared_ptr<AnimCircleHitTexture> createPinkAnimHitTexture() const {
        return std::make_shared<AnimCircleHitTexture>(AnimCircleHitTexture(*redEnemyTexture, 48, 48, 6, 0.1f));
    }
};



#endif //ENEMYSPRITEINJECTOR_H
