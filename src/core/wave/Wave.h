#ifndef WAVE_H
#define WAVE_H

#include "../src/config.h"
#include "../src/entity/Entity.h"

class Wave {
    private:
        std::vector<Entity> enemies;

    public:
        Wave();
        Wave(int n, GLFWwindow* window);

        void addEnemy(Entity& enemy);
        std::vector<Entity> &getEnemies();
};
#endif //WAVE_H