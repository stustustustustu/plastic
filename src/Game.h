#ifndef GAME_H
#define GAME_H

#include "config.h"
#include "core/camera/Camera.h"
#include "core/input/InputHandler.h"
#include "core/wave/manager/WaveManager.h"
#include "core/upgrades/manager/UpgradeManager.h"
#include "core/inventory/Inventory.h"
#include "core/island/Island.h"
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

    public:
        Game(float, float);
        ~Game();

        static Game *getInstance(float width = 1280.0f, float height = 960.0f);

        World *getCurrentWorld() const;
        void loadWorld(const std::string &path);
        void createNewWorld(unsigned int seed, Difficulty difficulty);

        std::vector<float> getSize();
        void setSize(const std::string &size);

        float getVolume() const;
        void setVolume(float volume);

        bool Init();
        void Update();
        void Render() const;
        void Loop();

        void resizeWindow(glm::ivec2 size);
};

#endif //GAME_H
