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
        GameState state;
        float width, height; // window dimensions;
        Difficulty difficulty;

    public:
        // rendering
        GLFWwindow *window;
        ShaderUtils *shader;
        Texture *texture;
        Renderer *renderer;

        // core systems
        WaveManager *wave;
        UpgradeManager *upgrade;
        Inventory *inventory;

        // island generation
        Island *generator;

        Entity player;
        std::vector<Turret> turrets;
        std::vector<Enemy> *enemies;

        Game(float, float);
        ~Game();

        std::vector<float> getSize();

        bool Init();
        void Update();
        void Render() const;
        void Loop();
};

#endif //GAME_H
