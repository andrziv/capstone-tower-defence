#ifndef WAVELOADER_H
#define WAVELOADER_H
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

#include "../special_enemy/normal/pink/PinkEnemy.h"
#include "../special_enemy/normal/red/RedEnemy.h"

class Enemy;

class WaveLoader {
    std::map<int, std::pair<int, nlohmann::basic_json<>>> waves;
    int currentWave = 0;
    int maxWaves = 0;

    static std::shared_ptr<Enemy> constructEnemy(const std::string &enemyName) {
        if(enemyName == "RED") {
            return std::make_shared<RedEnemy>(RedEnemy(nullptr));
        }
        if(enemyName == "BLUE") {
            return std::make_shared<BlueEnemy>(BlueEnemy(nullptr));
        }
        if(enemyName == "GREEN") {
            return std::make_shared<GreenEnemy>(GreenEnemy(nullptr));
        }
        if(enemyName == "YELLOW") {
            return std::make_shared<YellowEnemy>(YellowEnemy(nullptr));
        }
        if(enemyName == "PINK") {
            return std::make_shared<PinkEnemy>(PinkEnemy(nullptr));
        }
        return std::make_shared<RedEnemy>(RedEnemy(nullptr));
    }

    public:
        explicit WaveLoader(const std::string& file_path) {
            std::ifstream wavesFile(file_path);
            nlohmann::json waveFileJson = nlohmann::json::parse(wavesFile);
            wavesFile.close();
            auto wavesJson = waveFileJson.at("waves");
            auto wavesJsonIterator = wavesJson.items();
            int counter = 0;
            for (const auto& iteration_proxy_value : wavesJsonIterator) {
                auto item = iteration_proxy_value.value();
                auto wave_number = item.value("wave_number", 0);
                auto coin_reward = item.value("reward", 0);
                auto enemies_json = item.at("enemies");
                waves.insert(std::pair(wave_number, std::pair(coin_reward, enemies_json)));
                counter++;
            }
            currentWave = 1;
            maxWaves = counter;
        }

        std::vector<std::pair<float, std::shared_ptr<Enemy>>> getNextWave() {
            std::vector<std::pair<float, std::shared_ptr<Enemy>>> enemies;
            if (currentWave >= maxWaves) {
                return enemies;
            }
            for (const auto& iteration_proxy_value : waves.find(currentWave)->second.second.items()) {
                const auto& item = iteration_proxy_value.value();
                auto spawn_time = item.value("spawn_time_percent", 0);
                auto enemy_type = item.value("type", "none");
                enemies.emplace_back(std::pair(spawn_time, constructEnemy(enemy_type)));
            }
            currentWave++;
            return enemies;
        }

        [[nodiscard]] int getMaxWaves() const {
            return maxWaves;
        }

        [[nodiscard]] int getCurrentWave() const {
            return currentWave;
        }
};



#endif //WAVELOADER_H
