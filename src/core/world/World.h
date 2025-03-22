#ifndef WORLD_H
#define WORLD_H

#include "../src/config.h"

#include "../island/Island.h"
#include "../inventory/Inventory.h"
#include "../../entity/player/Player.h"
#include "../wave/manager/WaveManager.h"
#include "../../entity/explosion/Explosion.h"
#include "../upgrades/manager/UpgradeManager.h"
#include "../../entity/turret/manager/TurretManager.h"

#include "../replay/Replay.h"

enum Difficulty {
    EASY,
    MEDIUM,
    HARD,
    EXPERT,
    IMPOSSIBLE
};

class World {
    private:
        std::string name;
        unsigned int seed;
        Difficulty difficulty;

    public:
        std::unique_ptr<Replay> replay;

    public:
        std::unique_ptr<Player> player;
        std::unique_ptr<Island> island;
        std::unique_ptr<WaveManager> wave;
        std::unique_ptr<Inventory> inventory;
        std::unique_ptr<UpgradeManager> upgrade;
        std::unique_ptr<TurretManager> turret;

    public:
        std::vector<Enemy> *enemies;

        std::vector<std::unique_ptr<Projectile>> projectiles;
        std::vector<std::unique_ptr<Explosion>> explosions;

    public:
        World(std::string name, unsigned int seed, Difficulty difficulty);

        void init();

        void initial();
        void save(const std::string &world);
        void load(const std::string &world);

        void update();
        void render();

    public:
        std::string getName() const { return this -> name; }
        void setName(const std::string& name) { this -> name = name; }

        unsigned int getSeed() const { return this -> seed; }
        void setSeed(unsigned int seed) { this -> seed = seed; }

        Difficulty getDifficulty() const { return this -> difficulty; }
        void setDifficulty(Difficulty difficulty) { this -> difficulty = difficulty; }
};

#endif //WORLD_H
