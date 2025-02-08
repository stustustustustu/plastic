#ifndef WAVE_H
#define WAVE_H

#include "../src/config.h"
#include "../../entity/enemy/Enemy.h"

class Wave {
    private:
        std::vector<Enemy> enemies;
        int index;
        int weight;
        bool bossfight;

    public:
        Wave(int index, int weight, bool bossfight = false);

        void addEnemies(const std::vector<Enemy> &enemies);
        std::vector<Enemy> &getEnemies();

        int getIndex() const;
        void setIndex(int index);

        int getWeight() const;
        void setWeight(int weight);

        bool isBossfight() const;
        void setBossfight(bool bossfight);
};
#endif //WAVE_H