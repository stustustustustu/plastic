#ifndef WAVE_H
#define WAVE_H

#include "../src/config.h"
#include "../src/entity/Entity.h"
#include "../../entity/enemy/Enemy.h"

class Wave {
    private:
        std::vector<Enemy> enemies;

    public:
        Wave();
        Wave(int n, GLFWwindow* window);

        void addEnemy(Enemy& enemy);
        std::vector<Enemy> &getEnemies();
};
#endif //WAVE_H