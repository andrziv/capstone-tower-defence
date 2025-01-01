cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_Declare(json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(SFML json)

add_executable(main src/main.cpp
        src/entity/HitTexture.cpp
        src/pathing/Path.h
        src/entity/enemy/Enemy.cpp
        src/entity/enemy/Enemy.h
        src/entity/defence/tower/Tower.cpp
        src/entity/defence/tower/Tower.h
        src/entity/defence/projectile/Projectile.cpp
        src/entity/defence/projectile/Projectile.h
        src/entity/defence/projectile/tower_projectile/dev/DevProjectile.cpp
        src/entity/defence/projectile/tower_projectile/dev/DevProjectile.h
        src/helper/CollisionDetect.h
        src/GraphicsManager.cpp
        src/GraphicsManager.h
        src/GameManager.cpp
        src/GameManager.h
        src/entity/enemy/EnemyManager.cpp
        src/entity/enemy/EnemyManager.h
        src/entity/defence/tower/TowerManager.cpp
        src/entity/defence/tower/TowerManager.h
        src/entity/defence/projectile/ProjectileManager.cpp
        src/entity/defence/projectile/ProjectileManager.h
        src/helper/UUID.h
        src/entity/enemy/special_enemy/dev/DevEnemy.cpp
        src/entity/enemy/special_enemy/dev/DevEnemy.h
        src/entity/HitTexture.h
        src/pathing/Path.cpp
        src/entity/enemy/waves/WaveLoader.cpp
        src/entity/enemy/waves/WaveLoader.h
        src/entity/enemy/special_enemy/dev/SmallDevEnemy.cpp
        src/entity/enemy/special_enemy/dev/SmallDevEnemy.h
        src/entity/enemy/special_enemy/dev/MidDevEnemy.cpp
        src/entity/enemy/special_enemy/dev/MidDevEnemy.h
        src/entity/enemy/special_enemy/dev/LargeDevEnemy.cpp
        src/entity/enemy/special_enemy/dev/LargeDevEnemy.h
)
target_link_libraries(main PRIVATE SFML::Graphics)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)
