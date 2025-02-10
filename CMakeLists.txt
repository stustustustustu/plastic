cmake_minimum_required(VERSION 3.30)
project(plastic VERSION 0.1)

# todo: add font renderer using FreeType

find_package(
        OpenGL REQUIRED
)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        plastic
        src/main.cpp
        src/config.h

        src/glad.c

        src/utils/shader/ShaderUtils.cpp
        src/utils/shader/ShaderUtils.h
        src/utils/render/Renderer.cpp
        src/utils/render/Renderer.h

        src/entity/Entity.cpp
        src/entity/Entity.h
        src/entity/player/Player.cpp
        src/entity/player/Player.h
        src/entity/enemy/Enemy.cpp
        src/entity/turret/Turret.cpp
        src/entity/turret/Turret.h
        src/utils/render/sprites/Texture.cpp
        src/utils/render/sprites/Texture.h

        dependencies/stb/stb_image.cpp
        dependencies/stb/stb_perlin.cpp

        src/Game.cpp
        src/Game.h
        src/core/wave/Wave.cpp
        src/core/wave/Wave.h
        src/core/wave/manager/WaveManager.cpp
        src/core/wave/manager/WaveManager.h
        src/core/upgrades/Upgrade.cpp
        src/core/upgrades/Upgrade.h
        src/core/upgrades/manager/UpgradePath.cpp
        src/core/upgrades/manager/UpgradePath.h
        src/core/upgrades/manager/UpgradeManager.cpp
        src/core/upgrades/manager/UpgradeManager.h
        src/core/inventory/Inventory.cpp
        src/core/inventory/Inventory.h
        src/core/island/Island.cpp
        src/core/island/Island.h
        src/utils/math/noise/Perlin.cpp
        src/utils/math/noise/Perlin.h
        src/tilemap/Tile.h
        src/tilemap/Tilemap.cpp
        src/tilemap/Tilemap.h
        dependencies/stb/stb_perlin.cpp
        src/entity/enemy/Enemy.h
        src/utils/math/color/color.h
        src/utils/math/collision/Collision.cpp
        src/utils/math/collision/Collision.h
        src/core/UI/Element.cpp
        src/core/UI/Element.h
        src/core/camera/Camera.cpp
        src/core/camera/Camera.h
        src/utils/render/batch/BatchRenderer.cpp
        src/utils/render/batch/BatchRenderer.h
)

target_include_directories(plastic PRIVATE dependencies) # Includes location

target_link_directories(plastic PRIVATE dependencies/GLFW/library) # GLFW library location

target_link_libraries(plastic PRIVATE
        glfw3 # GLFW3 library

        kernel32 # System libraries
        user32
        gdi32
        winspool
        shell32
        ole32
        oleaut32
        uuid
        comdlg32
        advapi32
)
