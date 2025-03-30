#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H
#include <memory>

#include "SFML/Graphics/Sprite.hpp"


namespace sf {
    class RenderWindow;
    class Texture;
}

class AnimatedSprite {
    std::shared_ptr<sf::Sprite> sprite;
    int frameWidth;
    int frameHeight;
    int frameCount;
    float frameTime;
    float elapsedTime;
    int currentFrame;

public:
    AnimatedSprite(const sf::Texture &texture, int frameWidth, int frameHeight, const int frameCount, const float frameTime)
        : frameWidth(frameWidth), frameHeight(frameHeight), frameCount(frameCount),
          frameTime(frameTime), elapsedTime(0), currentFrame(0) {
        sprite = std::make_shared<sf::Sprite>(sf::Sprite(texture, sf::IntRect({0, 0}, {frameWidth, frameHeight})));
    }

    AnimatedSprite(const std::shared_ptr<sf::Sprite> &sprite, const int frameCount, const float frameTime)
        : frameWidth(sprite->getTextureRect().size.x), frameHeight(sprite->getTextureRect().size.y),
          frameCount(frameCount),
          frameTime(frameTime), elapsedTime(0), currentFrame(0) {
        this->sprite = sprite;
    }

    void update(const float deltaTime) {
        elapsedTime += deltaTime;
        if (elapsedTime >= frameTime) {
            elapsedTime = 0;
            currentFrame = (currentFrame + 1) % frameCount;
            sprite->setTextureRect(sf::IntRect({currentFrame * frameWidth, 0}, {frameWidth, frameHeight}));
        }
    }

    void setPosition(const float x, const float y) const {
        sprite->setPosition(sf::Vector2f(x, y));
    }

    [[nodiscard]] std::shared_ptr<sf::Sprite> getSprite() const {
        return sprite;
    }

    [[nodiscard]] sf::Vector2i getFrameSize() const {
        return {frameWidth, frameHeight};
    }

    [[nodiscard]] int getFrameCount() const {
        return frameCount;
    }

    [[nodiscard]] float getFrameTime() const {
        return frameTime;
    }
};


#endif //ANIMATEDSPRITE_H
