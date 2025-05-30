cmake_minimum_required(VERSION 3.30)
project(plastic VERSION 0.1)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

find_package(OpenGL REQUIRED)

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
        dependencies/stb/stb_truetype.cpp

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
        src/entity/enemy/Enemy.h
        src/utils/math/color/color.h
        src/utils/math/collision/Collision.cpp
        src/utils/math/collision/Collision.h
        src/core/UI/elements/Element.cpp
        src/core/UI/elements/Element.h
        src/core/camera/Camera.cpp
        src/core/camera/Camera.h
        src/utils/render/batch/BatchRenderer.cpp
        src/utils/render/batch/BatchRenderer.h

        dependencies/noise/FastNoise.cpp

        src/core/UI/elements/button/Button.cpp
        src/core/UI/elements/button/Button.h
        src/core/UI/elements/slider/Slider.cpp
        src/core/UI/elements/slider/Slider.h
        src/core/UI/elements/textbox/Textbox.cpp
        src/core/UI/elements/textbox/Textbox.h
        src/core/UI/elements/toggle/Toggle.cpp
        src/core/UI/elements/toggle/Toggle.h
        src/core/UI/elements/dropdown/Dropdown.cpp
        src/core/UI/elements/dropdown/Dropdown.h

        src/utils/render/text/TextRenderer.cpp
        src/utils/render/text/TextRenderer.h
        src/core/input/InputHandler.cpp
        src/core/input/InputHandler.h
        src/core/action/Action.cpp
        src/core/action/Action.h
        src/core/action/manager/ActionManager.cpp
        src/core/action/manager/ActionManager.h
        src/entity/turret/manager/TurretManager.cpp
        src/entity/turret/manager/TurretManager.h

        src/core/UI/scene/Scene.h
        src/core/UI/scene/Scene.cpp
        src/core/UI/scene/manager/SceneManager.h
        src/core/UI/scene/manager/SceneManager.cpp
        src/entity/projectile/Projectile.cpp
        src/entity/projectile/Projectile.h
        src/core/UI/scene/scenes/InGame.cpp
        src/core/UI/scene/scenes/InGame.h
        src/core/upgrades/panel/UpgradePanel.cpp
        src/core/upgrades/panel/UpgradePanel.h
        src/utils/math/number/number.h
        src/entity/turret/panel/ShopPanel.cpp
        src/entity/turret/panel/ShopPanel.h
        src/core/upgrades/turret/TurretUpgradeManager.cpp
        src/core/upgrades/turret/TurretUpgradeManager.h
        src/core/UI/scene/scenes/MainMenu.cpp
        src/core/UI/scene/scenes/MainMenu.h
        src/core/UI/scene/scenes/WorldCreation.cpp
        src/core/UI/scene/scenes/WorldCreation.h
        src/core/UI/scene/scenes/Settings.cpp
        src/core/UI/scene/scenes/Settings.h
        src/entity/explosion/Explosion.cpp
        src/entity/explosion/Explosion.h
        src/core/world/World.cpp
        src/core/world/World.h
        src/core/UI/scene/scenes/Pause.cpp
        src/core/UI/scene/scenes/Pause.h
        src/core/UI/scene/scenes/WorldSelection.cpp
        src/core/UI/scene/scenes/WorldSelection.h
        src/core/replay/Replay.cpp
        src/core/replay/Replay.h
        src/core/replay/manager/ReplayManager.cpp
        src/core/replay/manager/ReplayManager.h
        src/core/UI/scene/scenes/InReplay.cpp
        src/core/UI/scene/scenes/InReplay.h
        src/core/UI/scene/scenes/DeathScreen.cpp
        src/core/UI/scene/scenes/DeathScreen.h
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
