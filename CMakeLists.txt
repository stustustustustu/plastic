cmake_minimum_required(VERSION 3.30)
project(plastic VERSION 0.1)

# todo: add font renderer using FreeType

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        plastic
        src/main.cpp
        src/config.h

        src/glad.c

        src/utils/shader/ShaderUtils.cpp
        src/utils/shader/ShaderUtils.h
        src/utils/render/RenderUtils.cpp
        src/utils/render/RenderUtils.h
        src/utils/render/sprites/SpriteRenderer.cpp
        src/utils/render/sprites/SpriteRenderer.h

        src/utils/time/TimeUtils.cpp
        src/utils/time/TimeUtils.h

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
        src/Game.cpp
        src/Game.h
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
