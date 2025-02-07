#ifndef WAVEMANAGER_H
#define WAVEMANAGER_H

#include "../Wave.h"

class WaveManager {
    private:
        std::map<int, Wave> waves;

        int currentWaveIndex = 0;
        Wave *currentWave = NULL;

    public:
        void addWave(int index, const Wave &wave);
        bool removeWave(int index);

        Wave *getCurrentWave();
        int getWaveIndex(const Wave &wave) const;

        void startNextWave(GLFWwindow* window);
};

#endif //WAVEMANAGER_H
