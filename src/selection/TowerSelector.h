#ifndef TOWERSELECTOR_H
#define TOWERSELECTOR_H
#include <memory>
#include <vector>

#include "../entity/defence/tower/special_tower/bomb_tower/BombTower.h"
#include "../entity/defence/tower/special_tower/Jo_tower/JoTower.h"
#include "../entity/defence/tower/special_tower/radial_spreader/RadialShooterTower.h"
#include "SFML/System/Vector2.hpp"


class Tower;

class TowerSelector {

    const sf::Vector2f availIconStart = sf::Vector2f(1750.f, 100.f);
    const float availIconGap = 20.f;
    std::vector<std::shared_ptr<Tower>> availableTowers;
    std::shared_ptr<Tower> selectedTower;
    bool isTowerSelected;

    public:
        TowerSelector() {
            std::shared_ptr<Tower> towersToMakeAvail[] {
                std::make_shared<JoTower>(JoTower(sf::Vector2f(0, 0))),
                std::make_shared<RadialShooterTower>(RadialShooterTower(sf::Vector2f(0, 0))),
                std::make_shared<BombTower>(BombTower(sf::Vector2f(0, 0)))
            };

            int counter = 0;
            float switch_column = 0;
            float counter_y = 0;
            for (const auto &available : towersToMakeAvail) {
                available->setPosition(sf::Vector2f(
                    availIconStart.x + switch_column * (available->getHitTexture()->getRectDisplayEntity()->getSize().x + availIconGap),
                    availIconStart.y + counter_y * (available->getHitTexture()->getRectDisplayEntity()->getSize().y + availIconGap)));
                availableTowers.push_back(available);
                // check if even or odd
                if (counter % 2 == 0) { // even
                    switch_column = 1;
                } else { // odd
                    switch_column = 0;
                    counter_y++;
                }
                counter++;
            }

            isTowerSelected = false;
        }

        bool attemptSelectingTower(const sf::Vector2i& mousePosition) {
            if (!isTowerSelected) {
                for (const auto& tower : availableTowers) {
                    if (tower->getHitTexture()->getDisplayEntity()->getGlobalBounds().contains(sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)))) {
                        isTowerSelected = true;
                        selectedTower = tower->deep_copy(); // make a copy of the available tower
                        selectedTower->setId(get_uuid());
                        selectedTower->getHitTexture()->setDisplayEntityTransparency(0.5);
                        return true;
                    }
                }
            }
            return false;
        }

        void dragSelectedTower(const sf::Vector2i& mousePosition) const {
            if (isTowerSelected) {
                selectedTower->setPosition(
                    sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)));
            }
        }

        void deselectTower() {
            isTowerSelected = false;
            selectedTower->getHitTexture()->setDisplayEntityTransparency(1);
        }

        [[nodiscard]] bool doesSelectedTowerOverlap(const std::vector<std::shared_ptr<Tower>>& towers) const {
            for (const auto& tower : towers) {
                if (doRectanglesOverlap(*tower->getHitTexture()->getRectHitbox(), *selectedTower->getHitTexture()->getRectHitbox())) {
                    return true;
                }
            }
            return false;
        }

        [[nodiscard]] std::vector<std::shared_ptr<Tower>> getAvailableTowers() const {
            return availableTowers;
        }

        [[nodiscard]] bool isStillSelected() const {
            return isTowerSelected;
        }

        std::shared_ptr<Tower> getSelectedTower() {
            return selectedTower;
        }
};



#endif //TOWERSELECTOR_H
