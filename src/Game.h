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

enum Difficulty {
    EASY,
    MEDIUM,
    HARD,
    EXPERT,
    IMPOSSIBLE
};

class Game {
    private:
        static Game *instance;

        GameState state;
        float width, height; // window dimensions
        Difficulty difficulty;
        unsigned int seed;
        float volume = 0.5f;

    public:
        std::vector<std::unique_ptr<Projectile>> projectiles;
        std::vector<std::unique_ptr<Explosion>> explosions;

    public:
        // rendering
        GLFWwindow *window;
        ShaderUtils *shader;
        Texture *texture;
        Renderer *renderer;
        BatchRenderer *batch;
        TextRenderer *text;

        // entity managers
        TurretManager *turret;

        // camera
        Camera *camera;

        // core systems
        InputHandler *input;
        WaveManager *wave;
        UpgradeManager *upgrade;
        Inventory *inventory;
        SceneManager *scenes;

        // island generation
        Island *generator;

        // entities
        Player *player;
        std::vector<Turret> turrets;
        std::vector<Enemy> *enemies;

        Game(float, float);
        ~Game();

        static Game *getInstance(float width = 1280.0f, float height = 960.0f);

        std::vector<float> getSize();
        void setSize(const std::string &size);

        Difficulty getDifficulty() const;
        void setDifficulty(Difficulty difficulty);

        unsigned int getSeed() const;
        void setSeed(unsigned int seed);

        float getVolume() const;
        void setVolume(float volume);

        bool Init();
        void Update();
        void Render() const;
        void Loop();

        void resizeWindow(glm::ivec2 size);
};

#endif //GAME_H
