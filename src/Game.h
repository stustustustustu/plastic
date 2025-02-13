#ifndef GAME_H
#define GAME_H

#include "config.h"
#include "core/wave/manager/WaveManager.h"
#include "core/upgrades/manager/UpgradeManager.h"
#include "core/inventory/Inventory.h"
#include "core/island/Island.h"
#include "utils/render/Renderer.h"
#include "utils/render/sprites/Texture.h"
#include "utils/shader/ShaderUtils.h"

#include "entity/Entity.h"
#include "entity/player/Player.h"
#include "entity/turret/Turret.h"
#include "utils/render/batch/BatchRenderer.h"
#include "utils/render/text/TextRenderer.h"

enum GameState {
    ACTIVE,
    PAUSED,
    MENU,
    NONE
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

    public:
        // rendering
        GLFWwindow *window;
        ShaderUtils *shader;
        Texture *texture;
        Renderer *renderer;
        BatchRenderer *batch;
        TextRenderer *text;

        // core systems
        WaveManager *wave;
        UpgradeManager *upgrade;
        Inventory *inventory;

        // island generation
        Island *generator;

        // entities
        Entity player;
        std::vector<Turret> turrets;
        std::vector<Enemy> *enemies;

        Game(float, float);
        ~Game();

        static Game *getInstance(float width = 1280.0f, float height = 960.0f);

        std::vector<float> getSize();

        bool Init();
        void Update();
        void Render() const;
        void Loop();
};

#endif //GAME_H
