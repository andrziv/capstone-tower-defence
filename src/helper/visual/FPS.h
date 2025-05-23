#ifndef FPS_H
#define FPS_H
#include "SFML/System/Clock.hpp"
#include "SFML/System/Time.hpp"


class FPS {
public:
    /// @brief Constructor with initialization.
    ///
    FPS() : mFrame(0), mFps(0) {}

    /// @brief Update the frame count.
    ///


    /// @brief Get the current FPS count.
    /// @return FPS count.
    [[nodiscard]] unsigned int getFPS() const { return mFps; }

private:
    unsigned int mFrame;
    unsigned int mFps;
    sf::Clock mClock;

public:
    void update() {
        if(mClock.getElapsedTime().asSeconds() >= 1.f) {
            mFps = mFrame;
            mFrame = 0;
            mClock.restart();
        }

        ++mFrame;
    }
};

#endif //FPS_H
