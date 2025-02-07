#ifndef TOWERSELECTOR_H
#define TOWERSELECTOR_H
#include <vector>

#include "entity/defence/tower/Tower.h"
#include "entity/defence/tower/special_tower/Jo_tower/JoTower.h"


class TowerSelector {

    const sf::Vector2f availIconStart = sf::Vector2f(1550.f, 100.f);
    const float availIconGap = 10.f;
    std::vector<std::shared_ptr<Tower>> availableTowers;
    std::shared_ptr<Tower> selectedTower;
    bool isTowerSelected;

    public:
        TowerSelector() {
            std::shared_ptr<Tower> towersToMakeAvail[] {
                std::make_shared<JoTower>(JoTower(sf::Vector2f(0, 0)))
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
                if (counter & 1 == 0) { // even
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
                        return true;
                    }
                }
            }
            return false;
        }

        void dragSelectedTower(const sf::Vector2i& mousePosition) const {
            if (isTowerSelected) {
                selectedTower->setPosition(sf::Vector2f(
                    static_cast<float>(mousePosition.x) - selectedTower->getHitTexture()->getRectDisplayEntity()->getSize().x / 2,
                    static_cast<float>(mousePosition.y) - selectedTower->getHitTexture()->getRectDisplayEntity()->getSize().y / 2));
            }
        }

        void deselectTower() {
            isTowerSelected = false;
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
