#ifndef GAME_H
#define GAME_H

#include "config.h"
#include "core/camera/Camera.h"
#include "core/input/InputHandler.h"
#include "core/wave/manager/WaveManager.h"
#include "core/upgrades/manager/UpgradeManager.h"
#include "core/inventory/Inventory.h"
#include "core/island/Island.h"
#include "core/replay/manager/ReplayManager.h"
#include "core/UI/scene/manager/SceneManager.h"
#include "core/world/World.h"
#include "utils/render/Renderer.h"
#include "utils/render/sprites/Texture.h"
#include "utils/shader/ShaderUtils.h"

#include "entity/explosion/Explosion.h"
#include "entity/player/Player.h"
#include "entity/turret/Turret.h"
#include "entity/turret/manager/TurretManager.h"
#include "utils/render/batch/BatchRenderer.h"
#include "utils/render/text/TextRenderer.h"

enum GameState {
    ACTIVE,
    PAUSED
};

class Game {
    private:
        static Game *instance;

        std::unique_ptr<World> currentWorld;

        GameState state;
        float width, height; // window dimensions
        float volume = 0.5f;
        bool fullscreen = false;

        glm::ivec2 windowedPos;
        glm::ivec2 windowedSize;
        glm::ivec2 gameResolution = glm::ivec2(1280, 960);


    public:
        // rendering
        GLFWwindow *window;
        ShaderUtils *shader;
        Texture *texture;
        Renderer *renderer;
        BatchRenderer *batch;
        TextRenderer *text;

        // camera
        Camera *camera;

        // core systems
        InputHandler *input;
        SceneManager *scenes;
        ReplayManager *replay;

    public:
        Game(float, float);
        ~Game();

        static Game *getInstance(float width = 1280.0f, float height = 960.0f);

        World *getCurrentWorld() const;
        void loadWorld(const std::string &path);
        void createNewWorld(std::string name, unsigned int seed, Difficulty difficulty);

        GameState getState() const;
        void setState(GameState state);

        glm::vec2 getSize();
        void setSize(const std::string &size);
        void setSize(const glm::vec2 &size);

        float getVolume() const;
        void setVolume(float volume);

        bool getFullscreen() const;
        void setFullscreen(bool fullscreen);

        bool init();
        void update();
        void render() const;
        void loop();

        void handlePause();

        void resizeWindow(glm::ivec2 size);
};

#endif //GAME_H
